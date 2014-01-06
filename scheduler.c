#include "scheduler.h"

void scheduler_init(int p1in, int p2in, int p3in, int p4in)
{
	int i;

	// NB of objects
	nbP1 = p1in;
	nbP2 = p2in;
	nbP3 = p3in;
	nbP4 = p4in;
	
	// Initialize robots
	for(i=0; i < NB_ROBOT; i++)
		robot_init((i * 2) + 1);

	// start ring
	ring_init();

	// Start scheduler
	etat_tapis = 1;
	pthread_mutex_init(&etat_mutex, NULL);
	pthread_create(&th_tapis,NULL,&scheduler_main,NULL);
}

void *scheduler_main()
{
	int i;

	op = malloc(sizeof(pthread_t)*(nbP1 + nbP2 + nbP3 + nbP4));
	for(i=0;i < 1 ; i++)
		pthread_create(&op[i],NULL,&p1,NULL);

	while(etat_tapis != 2)
	{
		switch(etat_tapis)
		{
			case 0:
				// TODO join all thread
				printf("scheduler : Not in work !\n");
				break;
			case 1:
				printf("scheduler : In work !\n");
				break;
		}
		sleep(10);
	}
	return 0;
}

void scheduler_stop()
{
	pthread_mutex_lock(&etat_mutex);
	etat_tapis = 0;
	pthread_mutex_unlock(&etat_mutex);
}

void scheduler_start()
{
	pthread_mutex_lock(&etat_mutex);
	etat_tapis = 1;
	pthread_mutex_unlock(&etat_mutex);
}

void scheduler_finish()
{
	int i;

	pthread_mutex_lock(&etat_mutex);
	etat_tapis = 2;
	pthread_mutex_unlock(&etat_mutex);

	for(i=0;i<(nbP1 + nbP2 + nbP3 + nbP4);i++)
		pthread_join(op[i],NULL);
}

void *p1()
{
// TODO	describe the process
	tcom *msg = malloc(sizeof(tcom));
	com m;

	msg->order = GET;
	msg->qte = 3;
	msg->obj = C1;

	m.type = 1; // order
	m.data = msg;

	com_ecrire(m, tabRobot[0].idMsg);
	/*
	object_t o;
	o.etat = MATERIAL;
	o.type = C1;
	ring_putObject(PUT,&o);*/

// com_lire(*msg, tabRobot[x]);

// com_ecrire(msg, tabRobot[y]);
// do action
// com_lire(*msg, tabRobot[y]);
// etc..

// Get finish product

	return 0;
}

void *p2()
{
	return 0;
}

void *p3()
{
	return 0;
}

void *p4()
{
	return 0;
}
