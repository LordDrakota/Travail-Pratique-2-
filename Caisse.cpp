//--------------------------------------------------------
//
// Caisse.cpp
//
// D�finition de la classe Caisse qui permet de 
// cr�er une caisse et ajouter des clients
// par Jonathan Boucard & Pierre-Anthony Houle, 2016
//--------------------------------------------------------
#include "Caisse.h"
#include "Constantes.h"

Caisse::Caisse() : tempsFile(0), status(FERM�), nbClientServis(0), eteOuvert(false), totalAchats(0), nbClientsNonServis(0), tempsFileTotal(0)
{
}

void Caisse::AjouterClientFile(Client* client, float montantAchatClient)
{
	file.push_back(client);
	tempsFile += client->GetTempsAttenteClient(); 
	SetTempsFileTotal(GetTempsFileTotal() + client->GetTempsAttenteClient());
	SetNbClientsNonServis(GetNbClientsNonServis() +	1);
	if (client->GetTypeClient() != TYPECOMMERCIAL) client->SetMontantAchat(montantAchatClient);
	else
	{
		montantAchatClient -= (montantAchatClient * (client->GetPourcentageRabais()/100));
		client->SetMontantAchat(montantAchatClient);
	}
}

void Caisse::AfficherCaisse(ofstream& flux)
{
	flux << "--------------------------------------------------" << endl;
	flux << "@Status: ";
	if (GetStatus()) flux << "OUVERT" << endl; else flux << "FERM�" << endl;
	flux << "@� �t� ouverte: ";
	if (GetEteOuvert()) flux << "VRAI" << endl; else flux << "FAUX" << endl;
	flux << "@Nombre de clients servis: " << GetNbClientsServis() << endl;
	flux << "@Total des achats encaiss�s: " << GetTotalAchats() << endl;
	flux << "@Nombre de clients non servis: " << GetNbClientsNonServis() << endl;
	for (int i = 0; i < file.size(); i++)
	{
		file.at(i)->Afficher(flux);
	}
	flux << "@Temps d'attente � la fin: " << GetTempsFile() << endl;
	flux << "@Temps total d'attente: " << GetTempsFileTotal() << endl;
	flux << "--------------------------------------------------" << endl;
}

void Caisse::RetirerClientFile()
{
	tempsFile -= file.at(0)->GetTempsAttenteClient();
	SetTotalAchats(GetTotalAchats() + file.at(0)->GetMontantAchat());
	file.pop_front();
	SetNbClientsNonServis(GetNbClientsNonServis() - 1);
	SetNbClientsServis(GetNbClientsServis() + 1);
}



