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
    //Lyubovitsky parameterisation
    const G4double pA = 1;
    const G4double pAlpha = 1;
    const G4double g0 = 1;
    const G4double g = 1;
    const G4double c0 = 1;
    const G4double c = 1;
    const G4double beta_prime_pi = 1;
}

//Input Public Functions
G4ChargeExchangeNP::G4ChargeExchangeNP(){
    G4cout  << "G4ChargeExchangeNP Loaded" << G4endl;
    
    g4calc = G4Pow::GetInstance();
    auto table = G4ParticleTable::GetParticleTable();
    particleNeutron = table->FindParticle("neutron");
    particleProton = table->FindParticle("proton");
}

//Output Private Functions
G4double G4ChargeExchangeNP::GetElementCrossSection(const G4DynamicParticle* dp, G4int Z, const G4Material* mat)  
{
    G4double pE = dp->GetTotalEnergy();
    G4cout << "fEnergyLimit: "<< fEnergyLimit << " pE: " << pE << " Z: " << Z << G4endl;
    G4cout << "GetCrossSection: " << GetCrossSection(dp->GetDefinition(), mat, Z, pE) << G4endl;
    
    if (pE > fEnergyLimit){return GetCrossSection(dp->GetDefinition(), mat, Z, pE);}
    else{return 0;}
}

//-----------------------------------------------------------

G4double G4ChargeExchangeNP::GetCrossSection(const G4ParticleDefinition* part, const G4Material* mat, G4int ZZ, G4double pEtot){
    G4cout  << "GetCrossSection Loaded" << G4endl;
    
    const G4int Z = std::min(ZZ,ZMAXNUCLEARDATA);
    const G4int A = G4lrint(aeff[Z]);
    G4double SpecificSection = 0.0;
    G4int pdgN = part->GetPDGEncoding();

    G4double targetMass = CLHEP::proton_mass_c2;
    G4double projectileMass = part->GetPDGMass();
    G4double lorentz_s = targetMass*targetMass + 2*pEtot*targetMass + projectileMass*projectileMass;
    if(lorentz_s <= (targetMass + projectileMass)*(targetMass + projectileMass)){return 0;}

    //Calculations for Neutron Cross Section (NOT DONE YET)
    if (pdgN == 2112){
        G4double z23 = g4calc->Z23(Z);
    }


    G4cout  << "return function value: " << fFactor*SpecificSection + 10 << G4endl;
    return (fFactor*SpecificSection + 10);
}

const G4ParticleDefinition* G4ChargeExchangeNP::SampleSecondaryType(const G4ParticleDefinition* part, const G4Material* mat, G4int Z, G4int A, G4double etot){
    G4cout << "SampleSecondaryType loaded!";

    const G4ParticleDefinition* pd = nullptr;
    G4int pdgN = part->GetPDGEncoding();
    GetCrossSection(part, mat, Z, etot);

    //NOT DONE YET
    if (pdgN == 2112){
        G4double RandomNumberGen = (fFactor*SpecificSection)*(G4UniformRand());
        G4cout << "RandomNumberGen: " << RandomNumberGen << G4endl;
        if (RandomNumberGen > 0){
                pd = G4Proton::Proton();
        }
    }
    return pd;
}

G4double G4ChargeExchangeNP::SampleNeutron(const G4double etot, const G4double ltmax) const{
   return 0;
}