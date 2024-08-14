import numpy as np
import ROOT

data = np.loadtxt("./data_D0_daughtersPT_Eta_Phi_trk1_trk2_m1_m2_and_D0PT_Y_Phi_Mass.txt")
data = np.transpose(data)

holder = np.zeros([data.shape[1], 8])

for i in range(data.shape[1]):
    v_trk1 = ROOT.Math.PtEtaPhiMVector(data[0][i], data[2][i], data[4][i], data[6][i])
    v_trk2 = ROOT.Math.PtEtaPhiMVector(data[1][i], data[3][i], data[5][i], data[7][i])

    holder[i][0], holder[i][1] = v_trk1.Px(), v_trk2.Px() # Px (1st and 2nd columns)
    holder[i][2], holder[i][3] = v_trk1.Py(), v_trk2.Py() # Py (3rd and 4th columns)
    holder[i][4], holder[i][5] = v_trk1.Pz(), v_trk2.Pz() # Pz (5th and 6th columns)
    holder[i][6], holder[i][7] = v_trk1.E(), v_trk2.E() # Px (7th and 8th columns)

np.savetxt("data_D0_daughtersPx_Py_Pz_E_trk1_trk2.txt", holder)