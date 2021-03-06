// Main.cpp
// Jonathan Bouchard & Pierre-Anthony Houle
// 04 novembre 2016
// Cr�er une file de clients dans une instance de Quincaillerie

#include "Constantes.h"
#include "Quincaillerie.h"
#include "ClientParticulier.h"
#include "ClientPrivil�gi�.h"
#include "ClientCommercial.h"
#include "SourceLecture.h"
#include "Prototype.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

int main()
{
	locale::global(locale(""));			// Permet les charact�res fran�ais
	vector<Client*> vecClients;			// Vecteur contenant les clients
	SourceLecture fichierClients;		// Le fichier des clients
	SourceLecture fichierOp�rations;	// Le fichier des op�rations
	Quincaillerie magasin(NBCAISSES);	// La quincaillerie
	ofstream fichierEcriture;			// Le fichier du rapport final

	// Traiter le fichier client
	DemanderFichier(CLIENT, fichierClients, fichierEcriture);
	LireFichier(CLIENT, fichierClients, magasin, vecClients, fichierEcriture);
	// Traiter le fichier op�ration
	DemanderFichier(OPERATIONS, fichierOp�rations, fichierEcriture);
	LireFichier(OPERATIONS, fichierOp�rations, magasin, vecClients, fichierEcriture);
	// �crire le rapport final
	EcrireStatsFinales(magasin, fichierEcriture);
}

Client* GetClientByNum(vector<Client*> vecClients, int Num)
{
	Client* client;
	for (int i = 0; i < vecClients.size(); i++)
	{
		if (vecClients.at(i)->GetNumClient() == Num) client = vecClients.at(i);
	}
	return client;
}

#pragma region WriteFile
void StatusCaisse(Quincaillerie& magasin, ofstream& flux)
{
	for (int i = 0; i < magasin.GetVectorCaisse().size(); i++)
	{
		flux << "******************** Caisse " << i + 1 << " ********************" << endl;
		magasin.GetCaisse(i).AfficherCaisse(flux);
		flux << endl;
	}
}

void EcrireStatsFinales(Quincaillerie& magasin, ofstream& flux)
{
	int index;
	flux << "@@@@@@@@@@@@@@@@@@@@@ STATS @@@@@@@@@@@@@@@@@@@@@@" << endl;
	index = distance(magasin.GetVectorCaisse().begin(), magasin.GetCaissePlusClients()) + 1;
	flux << "Caisse qui a eu le plus grand nombre de clients: Caisse " << index << endl;
	magasin.GetVectorCaisse().begin(), magasin.GetCaissePlusClients()->AfficherCaisse(flux);
	index = distance(magasin.GetVectorCaisse().begin(), magasin.GetCaissePlusAttente()) + 1;
	flux << "Caisse o� il y a eu le plus d�attente: Caisse " << index << endl;
	magasin.GetVectorCaisse().begin(), magasin.GetCaissePlusAttente()->AfficherCaisse(flux);
	index = distance(magasin.GetVectorCaisse().begin(), magasin.GetCaissePlusArgent()) + 1;
	flux << "Caisse qui � encaiss� le plus d�argent: Caisse " << index << endl;
	magasin.GetVectorCaisse().begin(), magasin.GetCaissePlusArgent()->AfficherCaisse(flux);
	flux << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << endl;
}
#pragma endregion

#pragma region Quincaillerie
void CreerClient(vector<string> vectorElems, vector<Client*>& vecClient)
{
	if (vectorElems.at(1) == TYPEPARTICULIER) vecClient.push_back(new ClientParticulier(stoi(vectorElems.at(0)), vectorElems.at(1), vectorElems.at(2)));
	else if (vectorElems.at(1) == TYPEPRIVILEGIE) vecClient.push_back(new ClientPrivil�gi�(stoi(vectorElems.at(0)), vectorElems.at(1), vectorElems.at(2), vectorElems.at(3)));
	else if (vectorElems.at(1) == TYPECOMMERCIAL) vecClient.push_back(new ClientCommercial(stoi(vectorElems.at(0)), vectorElems.at(1), vectorElems.at(2), vectorElems.at(3), vectorElems.at(4), vectorElems.at(5), stoi(vectorElems.at(6))));
}

void ExecuterOperations(vector<string> vectorElems, Quincaillerie& magasin, vector<Client*> vecClient, ofstream& flux)
{
	try
	{
		if (vectorElems.at(0) == OUVRIRCAISSE) OuvrirCaisse(vectorElems, magasin, flux);
		else if (vectorElems.at(0) == AJOUTERCLIENT) AjouterClient(vectorElems, magasin, vecClient, flux);
		else if (vectorElems.at(0) == QUITTERCAISSE) QuitterCaisse(vectorElems, magasin, flux);
		else if (vectorElems.at(0) == FERMERCAISSE) FermerCaisse(vectorElems, magasin, flux);
		else throw exception("Type d'op�ration inconnu...");
		StatusCaisse(magasin, flux);
	}
	catch (exception e) { flux << e.what() << endl; }
}

void DemanderFichier(TypeFichier typeFichier, SourceLecture& fichier, ofstream& fecriture)
{
	string nomFichier;		// Variable qui retient le nom du fichier
	do
	{
		cout << "------------------------------------" << endl;
		if (typeFichier == CLIENT) cout << "Entrez un nom de fichier de clients: " << endl;
		else if (typeFichier == OPERATIONS) cout << "Entrez un nom de fichier d'op�rations: " << endl;
		cout << "------------------------------------" << endl;
		cin >> nomFichier;
		fichier.SetNomSourceLecture(nomFichier);
	} while (!fichier.EstCapableDeLire());
	if (typeFichier == OPERATIONS) fecriture.open("Journal_" + nomFichier);
}

void LireFichier(TypeFichier i, SourceLecture& fichier, Quincaillerie& magasin, vector<Client*>& vecClient, ofstream& flux)
{
	do
	{
		vector<string> vecElems;
		fichier.Lire(vecElems);
		if (i == CLIENT) CreerClient(vecElems, vecClient);
		else if (i == OPERATIONS) ExecuterOperations(vecElems, magasin, vecClient, flux);
	} while (fichier.PeutEncoreLire());
}
#pragma endregion 

#pragma region Operations
void OuvrirCaisse(vector<string> vectorElems, Quincaillerie& magasin, ofstream& flux)
{
	if (stoi(vectorElems.at(1)) - 1 >= 0 && stoi(vectorElems.at(1)) - 1 < NBCAISSES)
	{
		if (magasin.GetCaisse((stoi(vectorElems.at(1)) - 1)).GetStatus() != OUVERT)
		{
			magasin.GetCaisse((stoi(vectorElems.at(1)) - 1)).OuvrirCaisse();
			magasin.GetCaisse((stoi(vectorElems.at(1)) - 1)).SetEteOuvert(true);
			flux << "--OUVERTURE DE LA CAISSE #" << vectorElems.at(1) << endl;
		}
		else flux << "CAISSE #" << vectorElems.at(1) << " D�J� OUVERTE" << endl;
	}
	else throw exception("Le num�ro de la caisse est invalide!");
}

void AjouterClient(vector<string> vectorElems, Quincaillerie& magasin, vector<Client*> vecClient, ofstream& flux)
{
	Caisse* plusRapide;
	if (stoi(vectorElems.at(1)) - 1 >= 0 && stoi(vectorElems.at(1)) - 1 < vecClient.size())
	{
		Client* client = GetClientByNum(vecClient, stoi(vectorElems.at(1)));
		//// Ajoute le temps d'attente dans le client
		client->SetTempsClient(magasin.ConvertirMinuteEnSeconde(vectorElems.at(2)));

		//// Modifie la caisse (ajoute un client / ajoute le temps d'attente / **faire quelque chose avec le montant**)
		if (client->GetTypeClient() == TYPECOMMERCIAL) plusRapide = magasin.GetCaissePlusRapide(true);
		else plusRapide = magasin.GetCaissePlusRapide(false);

		plusRapide->AjouterClientFile(client, stof(vectorElems.at(3)));
		flux << "--AJOUT CLIENT #" << vectorElems.at(1) << endl;
	}
	else throw exception("Client Invalide!");
}

void QuitterCaisse(vector<string> vectorElems, Quincaillerie& magasin, ofstream& flux)
{
	if (stoi(vectorElems.at(1)) - 1 >= 0 && stoi(vectorElems.at(1)) - 1 < NBCAISSES)
	{
		if (magasin.GetCaisse(stoi(vectorElems.at(1)) - 1).GetFile().size() != 0)
		{
			magasin.GetCaisse(stoi(vectorElems.at(1)) - 1).RetirerClientFile();
			flux << "--TERMINER CAISSE #" << vectorElems.at(1) << endl;
		}
		else throw exception("Caisse Vide!");
	}
	else throw exception("Le num�ro de la caisse est invalide!");
}

void FermerCaisse(vector<string> vectorElems, Quincaillerie& magasin, ofstream& flux)
{
	if (stoi(vectorElems.at(1)) - 1 >= 0 && stoi(vectorElems.at(1)) - 1 < NBCAISSES)
	{
		if (magasin.GetCaisse((stoi(vectorElems.at(1)) - 1)).GetStatus() != FERM�)
		{
			magasin.GetCaisse((stoi(vectorElems.at(1)) - 1)).FermerCaisse();
			flux << "--FERMETURE DE LA CAISSE #" << vectorElems.at(1) << endl;
		}
		else flux << "CAISSE #" << vectorElems.at(1) << " D�J� FERM�" << endl;
	}
	else throw exception("Le num�ro de la caisse est invalide!");
}
#pragma endregion
