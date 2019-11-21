#include <stdio.h>
#include <stdlib.h>

#define N_CARDSET			1
#define N_CARD				52
#define N_DOLLAR			50


#define N_MAX_CARDNUM		13
#define N_MAX_USER			5
#define N_MAX_CARDHOLD		10
#define N_MAX_GO			17
#define N_MAX_BET			5

#define N_MIN_ENDCARD		30


//card tray object
int CardTray[N_CARDSET*N_CARD];
int cardIndex = 0;							


//player info
int dollar[N_MAX_USER];						//dollars that each player has
int n_user;									//number of users


//play yard information
int cardhold[N_MAX_USER+1][N_MAX_CARDHOLD];	//cards that currently the players hold
int cardSum[N_MAX_USER];					//sum of the cards
int bet[N_MAX_USER];						//current betting 
int gameEnd = 0; 							//����ִ� ī��(�迭�� 1�̸� 2��) 
int stop=0;									//stop�� 1�̸� each player turn�� ���� 

int numb[N_MAX_USER+1];						//����ִ� ī�� ���� 



int getIntegerInput(void) {
    int input, num;
    
    num = scanf("%d", &input);
    fflush(stdin);
    if (num != 1) //if it fails to get integer
        input = -1;
    
    return input;
}



//card processing functions ---------------

//calculate the actual card number in the blackjack game


int getCardNum(int cardnum) {
	
	
	int i;

	int number ; //ī���ȣ�� 1������ 13�����ִ°� 
	int cycle; //1���� 13���� 4�� ���� �� 

		cycle = cardnum / 13;
		number = cardnum % 13;
		
		if(number<10)
		{
			return (number+1);
		}
		else if(number>=10 && number<13)
		{
			return 10;
		}
			
}			



//print the card information (e.g. DiaA)
void printCard(int cardnum) {
	
	if(cardnum<52)
	{
		int pattern;
		pattern = cardnum / 13 ;
		
			switch(pattern)
			{
				case 0 : 
					printf("��");
					break;
				case 1 : 
					printf("��");
					break;
				case 2 : 
					printf("��");
					break;
				case 3 : 
					printf("��");
					break;
			}
		
		int number;
		number = cardnum % 13;
			switch(number)
			{
				case 0 :
					printf("A  ");
					break;
				case 10 :
					printf("J  ");
					break;
				case 11 :
					printf("Q  ");
					break;
				case 12 :
					printf("K  ");
					break;
				default :
					printf("%d  ",number+1);
					break;
			}
	}
	else
		printf(" ");
		
}	
	
//card array controllers -------------------------------

//mix the card sets and put in the array
int mixCardTray(void) {
	
	srand((unsigned)time(NULL));
	int temp=0;
	int random;
	int i;
	
	for(i=0;i<52;i++)  //ī��迭 52 
	{
		random = rand() %52;
		temp = CardTray[i];
		CardTray[i] = CardTray[random];
		CardTray[random] = temp;
	}
	printf("card is mixed and put into the tray");
}


//get one card from the tray
int pullCard(void) {
	
	int card;
	
	card = CardTray[cardIndex];
	cardIndex++;
	return card; //ī�� �ϳ��� ������ �� ���� ī�尡 ��µȴ�. 
}

//playing game functions -----------------------------

//player settiing
int configUser(void) {
	
	while(1)
	{
		printf("�÷��̾� ���� �Է��ϼ���.(�ִ�5): ");
		n_user = getIntegerInput();
		
			if(n_user<=N_MAX_USER && n_user>0)
				break;
			else 
				printf("1�� �̻�, 5�� ���Ϸ� �Է��ϼ���\n");	
	}
	return n_user;
}


//betting, 0�� ���̰� n_user�� ����, �������� �÷��̾���̴�. 
int betDollar(void) {
	int i;

	while(1)
	{
		printf("-----------------------------");
		printf("your betting (total $%d) : ",dollar[0]);
		bet[0] = getIntegerInput();
		
		if( bet[0] > dollar[0])
			printf("%d�޷� ������ �ݾ��� �Է��ϼ���", dollar[0]);	
		else
			break;
	}
	 
	for(i=1;i<n_user;i++)
	{
		printf("player%d bets $%d (�ѵ� : $%d)\n", i, bet[i], dollar[i]);
	}
		
		printf("\n--------------------------------\n");

	return 0;		
}	
	
//offering initial 2 cards
void offerCards(void) {
	int i;
	//1. give two card for each players
	for (i=0;i<n_user;i++)
	{
		cardhold[i][0] = pullCard();
		cardhold[i][1] = pullCard();
	}
	//2. give two card for the operator
	cardhold[n_user][0] = pullCard();
	cardhold[n_user][1] = pullCard();
	
	return;
}

//print initial card status
void printCardInitialStatus(void) {
		
	printf("-----------CARD OFFERING-----------");
	printf("\n----- server     :  ??  "); 
	printCard(cardhold[n_user][1]);
	numb[n_user]=2;
	
	printf("\n----- you        : "); 
	printCard(cardhold[0][0]);
	printCard(cardhold[0][1]);
	numb[0]=2;
	int i;

	for(i=1;i<n_user;i++)
	{
		printf("\n----- player %d   : ", i); 
		printCard(cardhold[i][0]);
		printCard(cardhold[i][1]);
		numb[i]=2;
	
	}
	printf("\n-----------------------------------------\n");
	return;
}


//���� ���� �� ���� ���ϰ� ����� ����Ͽ� ���ñ��� �����ϰų� ����	
int getAction(int playerNum) {  // playerNum don't change


	int go;				//go �Է��� �Է¹޴� �Լ� 

	int j,num=0,temp=0,cardVal=0;
	int isA=0;//is false
	
	//A�� 1���� 11���� ��� 
	for(j=0;;j++){
		if(cardhold[playerNum][j] == '\0' )
		{
			temp=cardSum[playerNum];
			if(temp ==21)
				break;
				 
			if(isA==1)//is true
			{
				while(temp < 21 && num > 0){
					temp += 10;
					if(temp > 21)
					{
						temp -=10;
						break;
					}
					num --;
				}
				cardSum[playerNum]=temp;
			}
			break;
		}
	}
	if (playerNum==0)
	{
		if(cardSum[playerNum]>21)
		{
			go=1;
			dollar[playerNum]=dollar[playerNum]-bet[playerNum];
			printf("DEAD (sum:%d) --> -$%d -> ($%d)", cardSum[playerNum], bet[playerNum], dollar[playerNum]);
			return -1 ;
		}
		else if(cardSum[0]==21 && numb[0]==2 )
		{
			dollar[0] = dollar[0] + 2*bet[0];
			printf("blackjack! YOU win!  -->  +$%d, ($%d)\n", bet[0], dollar[0]);
		}
		else
		{
			printf("Action? ( 0- go , 1 - stay) : ");
			scanf("%d",&go);
			
			if (go == 0)
			{
				cardhold[playerNum][numb[playerNum]]=pullCard();
				numb[playerNum]++;				
				return 0;
						
			}
			else if (go == 1)
			{
				stop = 1;
				return -1;
			}
		}
		
	}
	
	else if(playerNum<=n_user && playerNum > 0)
	{
		if (cardSum[playerNum] < 17)
		{
			go=0;
			printf("GO!\n"); 
			
			cardhold[playerNum][numb[playerNum]]=pullCard();
			numb[playerNum]++;
			return 0;		
		}
		else if(cardSum[playerNum] > 21)
		{
			go=1;
			dollar[playerNum]=dollar[playerNum]-bet[playerNum];
			printf("DEAD (sum:%d) --> -$%d -> ($%d)", cardSum[playerNum], bet[playerNum], dollar[playerNum]);
			return -1 ;
		}
		else
		{
			go=1;
			printf("STAY!\n");
			return -1;
		}
	}		 
	return 0;
}

void printUserCardStatus(int user, int cardcnt) {
	int i; 
	printf("   -> card : ");
	for (i=0;i<cardcnt;i++)
	{
		printCard(cardhold[user][i]);
	}
		printf("\t ::: ");
	return;
}

//A�� 1���� 11���� ����ϰ� ������ ����� ��� 
void calcStepResult(int playerNum) {
	 
if (playerNum == n_user)
	{	
		if(cardSum[playerNum]>21)
		{	
			printf("server DEAD (sum: %d)\n", cardSum[playerNum]);
			printf("[[[[server result is : .......overflow!! ]]]]");
			stop =1; 
			return;
		}
		else// if (cardSum[n_user]<=21)
			printf("[[[[[[[ server result is.... %d ]]]]]]]", cardSum[n_user]);
	}	
 return ;
}

//���� �÷��̾���� ī�� ���� ����ϸ� ������ ���Ͽ� ���и� ���� 
int checkResult() {
	
	int i;
	printf("----The result of YOU : ");
	
	//���� ���  
	if (cardSum[0]>=cardSum[n_user]&&cardSum[0]<=21) 
	{
		dollar[0]=dollar[0]+bet[0];
		printf("win! (sum:%d), --> : $%d\n",cardSum[0], dollar[0]);
	}
	else if (cardSum[0]<cardSum[n_user] && cardSum[0]<21)
	{
		dollar[0]=dollar[0]-bet[0];
		printf("lose! (sum:%d), --> : $%d\n",cardSum[0], dollar[0]); 
	}
	else if(cardSum[0]>21)	
	{
		printf("lose due to overflow! ($%d)\n" , dollar[0]);
	}
	
	//�÷��̾���� ���  
	for(i=1;i<n_user;i++)
	{	
		printf(" The result of player %d : ",i);
		if(cardSum[i]>=cardSum[n_user] && cardSum[i]<=21 )
		{	
			dollar[i]=dollar[i]+bet[i];
			printf("win! (sum:%d), --> : $%d\n",cardSum[i], dollar[i]);						
		}
		else if(cardSum[i]<cardSum[n_user]&& cardSum[i]<21) 
		{
			printf("lose! (sum:%d), --> : $%d\n",cardSum[i], dollar[i]);
		}
		else if(cardSum[i]>21)	
		{
			printf("lose due to overflow! ($%d)\n" , dollar[i]);
		}
	}
	return 0; 
}

// ���ñ��� ���� ����� ���ڰ� �ǵ��� ���� 
int checkWinner() {
	
	int i;
	int max;  //�ִ� ���� �� ��� 
	int num;	//�̱� �÷��̾��� ��ȣ�� ������ �� 
	printf("\n\n\n-------------------game end!-------------\n--> your money : $%d", dollar[0]);
	
	for(i=1;i<n_user;i++)
	{
		printf("\n--> player%d's money : $%d ",i,dollar[i]);
	}
	
	max=dollar[0];
	for(i=0;i<5;i++)
	{
		if(max<dollar[i])
		{
			max=dollar[i];
			num=i;
		}
	}	
	//���� �̰��� ���� �÷��̾ �̰��� ���� ���� 
	if (num==0)	
		printf("\n=======YOU win!======\n\n");
	else if(num>0 && n_user>1)
		printf("\n=======player %d win!=======\n\n",num); 
	

	printf("---------------Thanks for playing----------------");
}
	

	
	
int main(int argc, char *argv[]) {
	int roundIndex = 0;			//���� ���� ���� 
	int j,temp;					
	int action;					//getAction�� ���� ��ȯ�ϴ� ���� 
	int stop=0;					//stop���� 1�� �Է¹����� �� �÷��̾� ���� ���� 
	int sum;					//�� ������ ����ִ� ī�尪�� ���� �Ͻ������� ���� 
	
	
	printf("=============================================== WARNING ===================================================\n");
	printf("  ���� �ʹ� ī�� 2���� ������ �� �� ������ 1���� ī�带 ���� ���� �߻� �� ������ ��������ֽñ� �ٶ��ϴ�.\n");
	printf("                             overflow �Ǿ� ���ӿ��� ���� �÷��̾�� Ż���մϴ�.\n");
	printf("                             �� ������ ������ go�� �Ͽ��� ī�带 ���� �ʽ��ϴ�.\n");
	printf("===========================================================================================================\n");
	printf("���� ������ �����մϴ�.\n\n");
	//ī��Ʈ���� �迭�� ������� 0���� 51���� ��ȣ�� ����  ��
	for(j=0;j<N_CARDSET*N_CARD;j++)
	{
		CardTray[j]=j+1;	
	}


	srand((unsigned)time(NULL));
	
	//�÷��̾� �� ���� 
	configUser();

	//ī�带 �������� ���� 
	mixCardTray();


	//��� �����鿡�� �ʱ� ���ñ� 50�޷��� ����	
	for(j=0;j<n_user;j++)
	{
		dollar[j] = 50;
	}
	
	//Game start --------
	do {
		
		
		printf("\n\n\n------------------ Round %d (cardIndex : %d)--------------------------\n", roundIndex+1, cardIndex);
		
		//�� ���� ������ �÷��̾���� ���� �ݾ� �̸��� ���� ���� 
		for(j=1;j<n_user;j++) 
		{
			bet[j]=rand()%dollar[j]+1;
		} 
		
		//�� ���ñ� ����  
		betDollar();
		
		//ó���� ī�� 2�徿 ���� 
		offerCards(); 
		
		//ī�尡 ���� �����Ǹ� ���� �� 
		if(cardIndex > 52)
		{
			//gameEnd = 0;
			break;
		}
		
		printf("\n\n-----------------GAME START------------------------\n");
		
		//ó�� ī�� ���� 2�徿 �����ϰ� �ִ� �� ��� 
		printCardInitialStatus();
		
				
		int i;
		int m;
		
		//each player's turn
		for (i=0;i<=n_user;i++) //each player 
		{
			if(i==0)
				printf("\n\n\n>>>my turn! -----------------\n");
			else if( i>0 && i<n_user)
				printf("\n\n\n>>>player %d turn! ----------\n",i);
			else
				printf("\n\n\n>>>server turn! -------------\n");
			while ( stop==0 && cardSum[i] <= 21 ) //do until the player dies or player says stop
			{
				//ī�� ������ �ٴڳ��� ���� ���� 
				if (cardIndex>52)
					break;
				//���� �� ����ִ� ī�� �� ��� 	 
				for(m=0;m<numb[i];m++)
				{
					sum+=getCardNum(cardhold[i][m]);
				}
				cardSum[i] =sum;

				//sum�� 0���� ����� sum���� �� ���� ���� �������� ����. �ʱ�ȭ �ܰ� 
				sum=0;
				
				printUserCardStatus(i,numb[i]);
				
				//getAction���� ���� �����Ұ����� ���� �޾� ���� 
				//getAction�� ���� ��ȯ, YOU�� stop�� ��ġ�ų� �÷��̾ ���� ī���� ���� 17�� ������action ==-1 
				action=getAction(i);
				if (action == -1)
				{
					break;
				}
				 
				calcStepResult(i);
				
				//���ñ��� 0�� �Ǹ� ������ �ƿ� ���� 
				if (bet[i] == 0)
				{
					gameEnd = 0;
					break;	
				}
			}
		}
		
		printf("\n\n-------------ROUND %d result ...\n", roundIndex+1);
		//result
		checkResult();
		roundIndex++;
		
		
			
	} while (gameEnd == 0);
	

	checkWinner();
	
	
	return 0;
}

