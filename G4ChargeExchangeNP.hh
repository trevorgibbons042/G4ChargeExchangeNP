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
//
// -------------------------------------------------------------------
//
// GEANT4 Class header file G4ChargeExchangeXS
//
// Author: Trevor Gibbons 
 
// This is a class for charge exchange process on neutrons
// N0 + (N, Z, A) -> P + (N+1, Z-1, A)

#ifndef G4ChargeExchangeNP_h
#define G4ChargeExchangeNP_h 

#include "G4VCrossSectionDataSet.hh"
#include "globals.hh"
#include <vector>

class G4DynamicParticle;
class G4ParticleDefinition;
class G4Isotope;
class G4Element;
class G4Material;
class G4Pow;

class G4ChargeExchangeNP final : public G4VCrossSectionDataSet{
public:
    G4ChargeExchangeNP();
    ~G4ChargeExchangeNP() override = default;

    //Pointer Functions
    void SetCrossSectionFactor(G4double val) { fFactor = val; G4cout << "Printing factor:" << fFactor};

    G4double GetElementCrossSection(const G4DynamicParticle*, G4int Z, const G4Material*) final;

private:
    G4double GetCrossSection(const G4ParticleDefinition*, const G4Material*, G4int Z, G4double etot);

    //variables
    G4double fFactor{1.0};
}