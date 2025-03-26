import numpy as np
import ROOT
import random

data = np.loadtxt("./data_D0_daughtersPT1_PT2_Eta1_Eta2_Phi1_Phi2_M1_M2_and_D0PT_Y_Phi_Mass.txt")
nEvents = 20000

daughters = np.zeros([nEvents, 8])
mother = np.zeros([nEvents, 4])
verif = np.zeros([nEvents])

j = 0
while j < nEvents:
    i = random.randint(0, 3291859)

    if i not in verif:
        verif[j] = i

        v_trk1 = ROOT.Math.PtEtaPhiMVector(data[i][0], data[i][2], data[i][4], data[i][6])
        v_trk2 = ROOT.Math.PtEtaPhiMVector(data[i][1], data[i][3], data[i][5], data[i][7]) # Daughters

        daughters[j][0], daughters[j][1] = v_trk1.Px(), v_trk2.Px() # Px (1st and 2nd columns)
        daughters[j][2], daughters[j][3] = v_trk1.Py(), v_trk2.Py() # Py (3rd and 4th columns)
        daughters[j][4], daughters[j][5] = v_trk1.Pz(), v_trk2.Pz() # Pz (5th and 6th columns)
        daughters[j][6], daughters[j][7] = v_trk1.E(), v_trk2.E() # Px (7th and 8th columns)

        D0 = ROOT.Math.PtEtaPhiMVector(data[i][8], data[i][9], data[i][10], data[i][11]) # D0

        mother[j][0] = D0.Px() # Px (1st column)
        mother[j][1] = D0.Py() # Py (2nd column)
        mother[j][2] = D0.Pz() # Pz (3rd column)
        mother[j][3] = D0.E() # E (4th column)

        j += 1
    else:
        pass
        
np.savetxt("data_D0_daughtersPx_Py_Pz_E_trk1_trk2.txt", daughters, fmt='%.5f')
np.savetxt("data_D0_daughtersPx_Py_Pz_E_D0.txt", mother, fmt='%.5f')