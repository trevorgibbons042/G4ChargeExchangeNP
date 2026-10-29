Modified Geant4 11.4.1 files. run2.sh copies them to source/processes/hadronic/models/parton_string/diffraction/{src,include}/ and Geant4 is rebuilt and installed.
G4DiffractiveExcitation: G4DiffractiveExcitation::SetNoNeutronCE(true) turns off FTF quark exchange for neutron projectiles. Macro command: /CE/noFTFNeutronCE true (G4ChargeExchange sim, 3physics.cc).
