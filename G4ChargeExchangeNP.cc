//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// -------------------------------------------------------------------
//
// GEANT4 Class file
//
//
// File name: G4ChargeExchangeNP
// By: Trevor Gibbons
// Date Created: 9/7/26

#include "G4ChargeExchangeNP.hh"
#include "G4DynamicParticle.hh"
#include "G4ElementTable.hh"
#include "G4Material.hh"
#include "G4Element.hh"
#include "G4IsotopeList.hh"
#include "G4HadronicParameters.hh"
#include "Randomize.hh"
#include "G4SystemOfUnits.hh"
#include "G4NucleiProperties.hh"  
#include "G4Pow.hh"

#include "G4PionZero.hh"
#include "G4PionPlus.hh"
#include "G4Eta.hh"
#include "G4KaonZeroLong.hh"
#include "G4KaonZeroShort.hh"
#include "G4KaonPlus.hh"
#include "G4KaonMinus.hh"
#include "G4ParticleTable.hh"
#include "G4ThreeVector.hh"

#include "G4Proton.hh"

/*
Debug info: 
"G4ChargeExchangeNP Loaded"

"GetCrossSection Loaded"
"return function value: "

"SampleSecondaryType loaded!"
"RandomNumberGen: "
*/

/*
Notes:
GetCrossSection is set to constant value, no calculations yet.
*/

namespace{
    //Lyubovitsky parameterisation from Barton et al. 1976
    const G4double pA = 1523.8;
    const G4double pAlpha = .20096;
    const G4double g0 = 0.03129;
    const G4double g1 = -.0084418;
    const G4double c0 = -7.6817;
    const G4double c1 = 0.5507;
}

//Input Public Functions
G4ChargeExchangeNP::G4ChargeExchangeNP(){
    if (verboseLevel > 1) {G4cout  << "G4ChargeExchangeNP Loaded" << G4endl;}
    
    g4calc = G4Pow::GetInstance();
    auto table = G4ParticleTable::GetParticleTable();
    particleNeutron = table->FindParticle("neutron");
    particleProton = table->FindParticle("proton");
}

G4bool G4ChargeExchangeNP::IsElementApplicable(const G4DynamicParticle* , G4int Z, const G4Material*){return true;}
G4bool G4ChargeExchangeNP::IsIsoApplicable(const G4DynamicParticle* particle, G4int Z, G4int A, const G4Element*, const G4Material*){return false;}

//Output Private Functions
G4double G4ChargeExchangeNP::GetElementCrossSection(const G4DynamicParticle* dp, G4int Z, const G4Material* mat)  
{
    G4double pE = dp->GetTotalEnergy();
    
    if (verboseLevel > 1) {G4cout << "fEnergyLimit: "<< fEnergyLimit << " pE: " << pE << " Z: " << Z << G4endl;}
    
    if (Z == 1){return 0;}
    else if (pE > fEnergyLimit){return GetCrossSection(dp->GetDefinition(), mat, Z, pE);}
    else{return 0;}
}

//-----------------------------------------------------------

G4double G4ChargeExchangeNP::GetCrossSection(const G4ParticleDefinition* part, const G4Material* mat, G4int ZZ, G4double pEtot){
    
    if (verboseLevel > 1) {
    G4cout  << "GetCrossSection Loaded" << G4endl;}
    
    const G4int Z = std::min(ZZ,ZMAXNUCLEARDATA);
    const G4int A = G4lrint(aeff[Z]);
    G4double sum = 0;
    G4int pdgN = part->GetPDGEncoding();

    G4double targetMass = CLHEP::proton_mass_c2;
    G4double projectileMass = part->GetPDGMass();
    G4double lorentz_s = targetMass*targetMass + 2*pEtot*targetMass + projectileMass*projectileMass;
    
    if (verboseLevel > 1) {
    G4cout << "lorentz_s: "<<lorentz_s << G4endl << G4endl; }

    if(lorentz_s <= (targetMass + projectileMass)*(targetMass + projectileMass)){return 0;}

    //Calculations for Neutron Cross Section (NOT DONE YET)
    if (pdgN == 2112){
        G4double z23 = g4calc->Z23(Z);

        G4double S_S0 = lorentz_s*(1/(10*CLHEP::GeV*CLHEP::GeV));
        G4double logS_S0 = G4Log(S_S0);

        G4double Gtotal = std::max(0.0, 1+g0 + g1*logS_S0);
        G4double Ctotal = std::max(1e-16, 1+c0 + c1*logS_S0);
        G4double Afactortotal = pA*g4calc->powA(S_S0, 2*pAlpha-2);
        G4double Ztotal = z23*g4calc->powZ(Z, -.15*g4calc->powZ(Z,-2/3));

        G4double sumFactor = (10e-30)*Afactortotal*Ztotal*(1+Gtotal)/(Ctotal);

        if (verboseLevel > 1) {
        G4cout << "S_S0: "<< S_S0 << G4endl;
        G4cout << "g factor here!: " << Gtotal;
        G4cout << "c factor here!: " << Ctotal << G4endl;
        G4cout << "A factor here!: " << Afactortotal << G4endl;
        G4cout << "Z factor here!: " << Ztotal << G4endl;
        G4cout << "sum factor here:" << sumFactor << G4endl;
        G4cout  << "return function value: " << fFactor*sumFactor << G4endl;
        }
        return (fFactor*sumFactor);
    }
    else{return 0;}
}

const G4ParticleDefinition* G4ChargeExchangeNP::SampleSecondaryType(const G4ParticleDefinition* part, const G4Material* mat, G4int Z, G4int A, G4double etot){
    if (verboseLevel > 1) {
    G4cout << "SampleSecondaryType loaded!";}

    const G4ParticleDefinition* pd = nullptr;
    G4int pdgN = part->GetPDGEncoding();
    GetCrossSection(part, mat, Z, etot);
    pd = G4Proton::Proton();
    return pd;
}
