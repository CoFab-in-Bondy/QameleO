import numpy as np
import time

class FiltreKalman:
    def __init__(self):
        self.theta = np.array([[0.0], # biais init
                               [1.0]]) # gain init
                                
        
        self.P = np.array([[5.0, 0.0], # matrice de covariance
                           [0.0, 0.5]])
        
        self.Q = np.array([[0.001, 0.0], 
                           [0.0, 0.001]]) #matrice identité pour le bruit de processus

        self.R = np.array([[2.0]]) # bruit de mesure

    def estimation(self, pm_brut):
        
        pm_predit = (self.theta[1,0] * pm_brut) + self.theta[0,0]
        return pm_predit
    
    def mise_a_jour(self, pm_brut, pm_ref):
        
        # maj incertitude
        self.P = self.P + self.Q

        # matrice d'observation
        H = np.array([[1.0, pm_brut]])

        # innovation (différence entre ref et estimation)
        pm_predit = self.estimation(pm_brut)
        erreur = pm_ref - pm_predit

        # incertitude totale de l'étape
        S = np.dot(np.dot(H, self.P), H.T) + self.R

        # à quel point on corrige le biais et le gain
        K = np.dot(self.P, H.T) / S[0,0]

        # correction finale
        self.theta = self.theta + (K*erreur)

        # réduction de l'incertitude
        I = np.array([[1,0],
                      [0,1]])
        self.P = np.dot((I - np.dot(K,H)), self.P)


# Gestion de l'arrivée séquentielle des données

class ReceptionDonnees:
    def __init__(self):
        self.storage = {
            "AIR_QAM05": None,
            "AIR_QAM01": None,
            "AIR_QAM03": None
        }
        self.filtre = FiltreKalman()

    def on_reception(self, id_capteur, valeur_pm):

        valeur_corrigee = None

        if id_capteur == "AIR_QAM05":
            self.storage['AIR_QAM05'] = valeur_pm
            valeur_corrigee = self.filtre.estimation(valeur_pm)
            

        elif id_capteur == "AIR_QAM01":
            self.storage['AIR_QAM01'] = valeur_pm
        
        elif id_capteur == "AIR_QAM03":
            self.storage['AIR_QAM03'] = valeur_pm
        
        if (self.storage["AIR_QAM05"] is not None and
            self.storage["AIR_QAM01"] is not None and
            self.storage["AIR_QAM03"] is not None):

            ref_moyenne = (self.storage['AIR_QAM01']+self.storage['AIR_QAM03'])/2
            self.filtre.mise_a_jour(self.storage['AIR_QAM05'], ref_moyenne)

            self.storage["AIR_QAM01"] = None
            self.storage["AIR_QAM03"] = None
            self.storage["AIR_QAM05"] = None

        return valeur_corrigee

    
