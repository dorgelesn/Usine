#include "communication.h"

int com_init()
{
	key_t cle;
	int msgid;

	/*  Creation de la clé IPC */
	 if ((cle = ftok(".", 'A')) == -1) 
	{
		perror("Erreur de creation de la clé \n");
		exit(1);
	 }

	/* Creation de la file de message 	*/
	if ((msgid = msgget(cle, IPC_CREAT | IPC_EXCL | 0600)) == -1) 
	{
		if ((msgid = msgget (cle,0)) == -1)
		{
			perror("Erreur de creation de la file\n");
			exit(1);
		}
   }
	return msgid;
}

void com_removeFile(int msgid)
{
	msgctl(msgid, IPC_RMID,NULL);
}

void com_ecrire(com* message, int msgid)
{
//	key_t key;
   printf("Send : %d %d %d\n", message->data->order, message->data->obj, message->data->qte);
	test = message->data;
	/* Envoie du message sur la file	*/
	if (msgsnd(msgid, &message, sizeof(com),0) == -1) 
	{
		perror("Erreur d'ecriture requete : ");
		exit(1);
	}
	printf("Message envoyé\n");
}

com *com_lire(int msgid, msgType m)
{
	com *message = malloc(sizeof(com));
	int longMSG;
	
	/* lecture du message sur la file */
	if ((longMSG = msgrcv(msgid, message, sizeof(com), m, 0)) == -1) 
	{
		perror("Erreur de lecture requete : ");
		exit(1);
	}
	printf("Message lu : %d\n", longMSG);
printf("test : %d\n",test);
printf("mess : %d\n",message->data);
printf("Receive : %d %d %d\n",message->data->order, message->data->obj, message->data->qte);
   return message;
}

/**
 * Envoie un ordre à un robot
 */
void com_sendOrder(int msgid, tcom *t)
{
	com *m = malloc(sizeof(com));
	m->type = ORDER;
	m->data = t;
	
	com_ecrire(m,msgid);
}

/**
 * Le robot recoit un ordre
 */
tcom* com_getOrger(int msgid)
{
	com* t = com_lire(msgid,ORDER);
	return t->data;
}

/**
 * Envoie un ACK au scheduler
 */
void com_sendACK(int msgid)
{
	com* m = malloc(sizeof(com));
	m->type = ACK;
	m->data = NULL;
	
	com_ecrire(m,msgid);
}

/**
 * Le scheduler recoit un ACK
 */
void com_waitACK(int msgid)
{
	com_lire(msgid,ACK);
}