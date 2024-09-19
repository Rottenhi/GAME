#define CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "header.h"

void story1(const Character* hero) {
	printf("You were knocked out.\n\n");
	printf("You awake in a small hut in a small forest next to a girl.\n");
	printf("(Girl): You are finally awake.\n");
	printf("You are being healed by the girl\n");
	printf("(Girl): Tought you might not wake up. You got hit pretty good.\n");
	printf("(%s): Where am I? Who are you? What is going on?\n", hero->name);
	printf("(Seilna): First of all I am Seilna, and you are in the forest of Altranta.\n");
	printf("(Seilna): As for what is going on... I am hiring you.\n");
	printf("(Seilna): I need an escort to the continent of Halon\n");
	printf("(%s): I just woke up and you want me to escort you?\n",hero->name);
	printf("(Seilna): Yes?\n");
	printf("(%s): Fine, fetch my sword and we can go.\n", hero->name);
	printf("\nYou left the forest towards the west for port town Nilga\n");
	printf("\nYou encounter an enemy!\n");
}
void story2(const Character* hero) {
	printf("(%s): That was close we almost died to the knight's sword.\n", hero->name);
	printf("(Seilna): Don't mention it anymore. We are lucy to be alive!\n");
	printf("(%s): Yes let's go to Nilga\n", hero->name);
	printf("\nSeilna and you reach Nilga where you find a cheap ship and board it.\n");
	printf("\nThe story ends here for now. While our hero and the girl travel to the other continent.\n");
}