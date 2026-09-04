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

/*
Debug info: 
"G4ChargeExchangeNP Loaded"
"G4SAMPLESECONDARYTYPE IS WORKING!!"
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

//Output Private Functions
G4double G4ChargeExchange::GetElementCrossSection(const G4DynamicParticle* dp, G4int Z, const G4Material* mat)  
{
    G4double pE = dp->GetTotalEnergy();
    G4cout << "fEnergyLimit: "<< fEnergyLimit << " pE: " << pE << " Z: " << Z << G4endl;
    G4cout << "GetCrossSection: " << GetCrossSection(dp->GetDefinition(), mat, Z, pE) << G4endl;
    
    if (pE > fEnergyLimit){
        return GetCrossSection(dp->GetDefinition(), mat, Z, pE)
        }
    else{
        return 0;
    }
}

//-----------------------------------------------------------
G4ChargeExchangeXS::G4ChargeExchangeXS(){
    G4cout  << "G4ChargeExchangeNP Loaded" << G4endl;
    
    g4calc = G4Pow::GetInstance();
    auto table = G4ParticleTable::GetParticleTable();
    particleNeutron = table->FindParticle("neutron");
    particleProton = table->FindParticle("proton");
}

G4double G4ChargeExchangeNP::GetCrossSection(const G4ParticleDefinition* part, const G4Material* mat, G4int ZZ, G4double pEtot){
    const G4int Z = std::min(ZZ,ZMAXNUCLEARDATA);
    const G4int A = G4lrint(aeff[Z]);
    G4double CrossSection = 0.0;
    G4int pdgN = part->GetPDGEncoding();

    G4double targetMass = CLHEP::proton_mass_c2;
    G4double projectileMass = part->GetPDGMass();
    G4double lorentz_s = targetMass**2 + 2*projectileMass*targetMass + projectileMass**2
    if(lorentz_s <= (targetMass + projectileMass)**2){return 0;}

    //Calculations for Neutron Cross Section (NOT DONE YET)
    if (pdgN == 2112){
        G4double Z23 = g4calc->Z23(Z);
    }









    return fFactor*CrossSection + 10
}

const G4ParticleDefinition* G4ChargeExchangeXS::SampleSecondaryType(const G4ParticleDefinition* part, const G4Material* mat, G4int Z, G4int A, G4double etot){
    G4cout << "G4SAMPLESECONDARYTYPE IS WORKING!!";

    const G4ParticleDefinition* pd = nullptr;
    G4int pdgN = part->GetPDGEncoding();
    GetCrossSection(part, mat, Z, etot);

    //NOT DONE YET
    if (pdgN == 2112){
        G4double RandomNumberGen = (fFactor*CrossSection)*(G4UniformRand());
        G4cout << "RandomNumberGen: " << RandomNumberGen << G4endl;
        if (pdgN==2112){
                pd = G4Proton::G4Proton();
        }
    }
    return pd;
}