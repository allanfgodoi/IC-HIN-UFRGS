# ## Adapted code from Breno Orzari, "VAE_Gauss".
# 
# Available in: https://colab.research.google.com/drive/1zfmanIeKsla96cI7LgPhebqz0wuJ4qzZ?usp=sharing

import timeit
start_time = timeit.default_timer()
import torch
import torchvision
import torchvision.transforms as transforms
import torch.nn as nn
import torch.nn.functional as F
import numpy as np
from torchvision.utils import save_image
import matplotlib.pyplot as plt
from scipy.stats import kstest
import optuna
from optuna.trial import TrialState
device = torch.device('cuda' if torch.cuda.is_available() else 'cpu')
print(device)

# $H_0$: the samples have the same distribution
# $H_A$: the samples have not the same distribution

def critic_ks_test(D, n):
    # table of critical values for the kolmogorov-smirnov test - 95% confidence
    # Source: https://www.soest.hawaii.edu/GG/FACULTY/ITO/GG413/K_S_Table_one_Sample.pdf
    # Source: http://www.real-statistics.com/statistics-tables/kolmogorov-smirnov-table/
    # alpha = 0.05 (95% confidential level)
    
    if n <= 40:
        ks_list = [0.90000, 0.68377, 0.56481, 0.49265, 0.44697, 0.41035, 0.38145, 0.35828, 0.33907, 0.32257, 
                      0.30826, 0.29573, 0.28470, 0.27481, 0.26588, 0.25778, 0.25039, 0.24360, 0.23735, 0.23156, 
                      0.22617, 0.22115, 0.21645, 0.21205, 0.20790, 0.20399, 0.20030, 0.19680, 0.19348, 0.19032, 
                      0.18732, 0.18445, 0.18171, 0.17909, 0.17659, 0.17418, 0.17188, 0.16966, 0.16753, 0.16547,
                      0.16349, 0.16158, 0.15974, 0.15796, 0.15623, 0.15457, 0.15295, 0.15139, 0.14987, 0.14840]
        critic_ks = ks_list[n - 1]
    elif n > 50:
        critic_ks = 1.07275/(np.sqrt(n))
    else:
        pass            
            
    if critic_ks >= D:
        return "DO NOT REJECT H0"
    else:
        return "REJECT H0"

# Read dpt distribution file as numpy array and then converts to pytorch tensor
x_daughters = np.loadtxt('./../Skims/data_D0_daughtersPT_trk1_trk2.txt')
x_daughters = torch.from_numpy(x_daughters)
x_daughters = x_daughters.to(torch.float32)

# Dividing our data to train/eval/test (50/25/25)%
x_daughters_div = torch.tensor_split(x_daughters, 2)
x_train = x_daughters_div[0]
x_val, x_test = torch.tensor_split(x_daughters_div[1], 2)

nTrain = x_train.size()[0]
nVal = x_val.size()[0]
nTest = x_test.size()[0]
nVar = x_daughters.size()[1]

def objective(trial):
    # ATUALIZADO 2ª RUN
    nHidden = trial.suggest_int("nHidden", 2200, 2210)
    nLatent = trial.suggest_int("nLatent", 205, 215)

    lr = 2e-3
    batch_size = 15
    epochs = 15

    nBins = 60
    r = [1, 3.5]

    # Defining our DataLoaders
    trainloader = torch.utils.data.DataLoader(x_train, shuffle=True, batch_size=batch_size) # Train
    valloader = torch.utils.data.DataLoader(x_val, shuffle=False, batch_size=batch_size) # Eval
    testloader = torch.utils.data.DataLoader(x_test, shuffle=False, batch_size=batch_size) # Test

    # VAE class with inheritance from nn.Module
    class VAE(nn.Module):
        def __init__(self):
            super(VAE, self).__init__()

            # Autoencoding architeture
            self.fc1 = nn.Linear(nVar, nHidden) # Encoder
            self.fc2 = nn.Linear(nHidden, nHidden) # Encoder
            self.fc21 = nn.Linear(nHidden, nLatent) # Mean
            self.fc22 = nn.Linear(nHidden, nLatent) # Std

            self.fc3 = nn.Linear(nLatent, nHidden) # Decoder
            self.fc31 = nn.Linear(nHidden, nHidden) # Decoder
            self.fc4 = nn.Linear(nHidden, nVar) # Decoder

        # Encoding func
        def encoder(self, x):
            h = torch.tanh(self.fc1(x))
            #h = torch.tanh(self.fc2(h))
            return self.fc21(h), self.fc22(h) # Mean, Std

        def sampling(self, mu, std): # Reparameterization trick
            eps = torch.randn_like(std)
            return 0.5*((eps * std + mu)) # Using two samples to compute expectation over z

        def decoder(self, z):
            h = torch.tanh(self.fc3(z))
            return self.fc4(h)

        def forward(self, x):
            mu, std = self.encoder(x.view(-1, nVar))
            z = self.sampling(mu, std)
            return self.decoder(z), mu, std
    model = VAE() # Defining VAE as our model

    optimizer = torch.optim.Adam(model.parameters(), lr = lr) # Implements Adam algorithm to our model parameters (Adam algorithm is a alternative to the classic stochastic gradient descent to update the weights based on
    # training dataset)
    scheduler = torch.optim.lr_scheduler.ReduceLROnPlateau(optimizer, 'max', factor=0.5, # Applying a scheduler to the optimizer
                                patience=5, threshold=0.001, cooldown=0,
                                min_lr=0.0001, verbose=True)

    # Defining the loss fucntion
    def loss_function(y, x, mu, std):
        loss = nn.MSELoss(reduction='sum') # The reduction='sum' parameters return the sum of all MSE
        ERR = loss(y, x)
        KLD = -0.5 * torch.sum(1 + torch.log(std**2) - mu**2 - std**2) # KL Divergence
        return ERR + KLD, -ERR, -KLD

    count=0
    err_l, kld_l, n_wu, terrl, tkldl, update = [], [], [], [], [], []
    for epoch in range(epochs):

        train_loss = 0
        for batch_idx, (data) in enumerate(trainloader): # batch_idx = iteration index
            model.train()
            bsize = data.shape[0] # Define a variable with the size of epoch's minibatch
            recon_batch, mu, std = model(data) # Call our model
            loss, err, kld = loss_function(recon_batch, data, mu, std) # Call the loss function and the KLD
            loss.backward() # Compute tensor gradient
            train_loss += err.item() + kld.item() # torch.Tensor.item(): returns the value from a single-valued tensor
            optimizer.step() # Update optimizer parameters
            optimizer.zero_grad() # Reset tensor gradient
            err_l.append(err.item()/bsize) # minibatch mean error
            kld_l.append(kld.item()/bsize) # minibatch mean KLD
            count+=1
            n_wu.append(count)

            if (count%100 == 0): # Validating every 100 weight updates
                for batch_idx, datav in enumerate(valloader):
                    model.eval()
                    trecon, tmu, tstd = model(datav)
                    tloss, terr, tkld = loss_function(trecon, datav, tmu, tstd)
                    terrl.append(terr.item()/datav.shape[0])
                    tkldl.append(tkld.item()/datav.shape[0])
                    update.append(count)

        scheduler.step(train_loss / len(trainloader.dataset)) # Do the scheduler step

        print('====> Epoch: {} Average loss: {:.4f}'.format(epoch, train_loss / len(trainloader.dataset)))
        model.eval()
        test_loss= 0
        with torch.no_grad(): # Disables gradient calculation to reduce the computational cost
            for data in testloader:
                recon, mu, std = model(data)
                loss, err, kld = loss_function(recon, data, mu, std)
                test_loss += err + kld

        test_loss /= len(testloader.dataset)
        print(f'Error = {err_l[-1]:.4f} | KLD = {kld_l[-1]:.4f} | Loss = {test_loss:.4f}')

    model.eval()
    a = x_train
    recon, mu, std = model(a)
    b = recon

    a = a.numpy()
    b = b.detach().numpy()
    a = a.flatten()
    b = b.flatten()
    D, ks_significance = kstest(b, a, N=len(b))
    verifier = critic_ks_test(D, len(b))
    print(verifier)

    return D

def main():

    study = optuna.create_study(direction="minimize")
    study.optimize(objective, n_trials=None, timeout=21600)

    pruned_trials = study.get_trials(deepcopy=False, states=[TrialState.PRUNED])
    complete_trials = study.get_trials(deepcopy=False, states=[TrialState.COMPLETE])

    print("Study statistics: ")
    print("  Number of finished trials: ", len(study.trials))
    print("  Number of pruned trials: ", len(pruned_trials))
    print("  Number of complete trials: ", len(complete_trials))

    print("Best trial:")
    trial = study.best_trial

    print("  Value: ", trial.value)

    print("  Params: ")
    for key, value in trial.params.items():
        print("    {}: {}".format(key, value))

    fig1 = optuna.visualization.plot_optimization_history(study)
    fig1.show()
    fig2 = optuna.visualization.plot_slice(study, params=["nHidden", "nLatent"])
    fig2.show()
    fig3 = optuna.visualization.plot_param_importances(study)
    fig3.show()

    # Elapsed time
    elapsed = timeit.default_timer() - start_time
    print(f"{int(elapsed)} s\n")

if __name__=='__main__':
    main()
