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
int gameEnd = 0; 							//들고있는 카드(배열상 1이면 2개) 
int stop=0;									//stop이 1이면 each player turn이 끝남 

int numb[N_MAX_USER+1];						//들고있는 카드 개수 



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

	int number ; //카드번호는 1번부터 13까지있는거 
	int cycle; //1부터 13까지 4번 도는 거 

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
					printf("♥");
					break;
				case 1 : 
					printf("◆");
					break;
				case 2 : 
					printf("♠");
					break;
				case 3 : 
					printf("♣");
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
	
	for(i=0;i<52;i++)  //카드배열 52 
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
	return card; //카드 하나씩 꺼내면 그 다음 카드가 출력된다. 
}

//playing game functions -----------------------------

//player settiing
int configUser(void) {
	
	while(1)
	{
		printf("플레이어 수를 입력하세요.(최대5): ");
		n_user = getIntegerInput();
		
			if(n_user<=N_MAX_USER && n_user>0)
				break;
			else 
				printf("1명 이상, 5명 이하로 입력하세요\n");	
	}
	return n_user;
}


//betting, 0이 나이고 n_user가 딜러, 나머지는 플레이어들이다. 
int betDollar(void) {
	int i;

	while(1)
	{
		printf("-----------------------------");
		printf("your betting (total $%d) : ",dollar[0]);
		bet[0] = getIntegerInput();
		
		if( bet[0] > dollar[0])
			printf("%d달러 이하의 금액을 입력하세요", dollar[0]);	
		else
			break;
	}
	 
	for(i=1;i<n_user;i++)
	{
		printf("player%d bets $%d (한도 : $%d)\n", i, bet[i], dollar[i]);
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


//게임 시작 후 고스톱 정하고 결과를 계산하여 배팅금을 차감하거나 더함	
int getAction(int playerNum) {  // playerNum don't change


	int go;				//go 입력을 입력받는 함수 

	int j,num=0,temp=0,cardVal=0;
	int isA=0;//is false
	
	//A가 1일지 11일지 계산 
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

//A가 1일지 11일지 계산하고 딜러의 결과를 출력 
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

//현재 플레이어들의 카드 합을 계산하며 딜러와 비교하여 승패를 가림 
int checkResult() {
	
	int i;
	printf("----The result of YOU : ");
	
	//나의 경우  
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
	
	//플레이어들의 경우  
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

// 배팅금이 많은 사람이 승자가 되도록 결정 
int checkWinner() {
	
	int i;
	int max;  //최댓값 구할 때 사용 
	int num;	//이긴 플레이어의 번호를 가져올 것 
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
	//내가 이겼을 때와 플레이어가 이겼을 때를 구분 
	if (num==0)	
		printf("\n=======YOU win!======\n\n");
	else if(num>0 && n_user>1)
		printf("\n=======player %d win!=======\n\n",num); 
	

	printf("---------------Thanks for playing----------------");
}
	

	
	
int main(int argc, char *argv[]) {
	int roundIndex = 0;			//라운드 진행 개수 
	int j,temp;					
	int action;					//getAction의 값을 반환하는 변수 
	int stop=0;					//stop값을 1로 입력받으면 각 플레이어 턴이 종료 
	int sum;					//각 유저별 들고있는 카드값의 합을 일시적으로 저장 
	
	
	printf("=============================================== WARNING ===================================================\n");
	printf("  게임 초반 카드 2장을 나눠줄 때 한 유저만 1개의 카드를 갖는 오류 발생 시 게임을 재시작해주시기 바랍니다.\n");
	printf("                             overflow 되어 게임에서 지면 플레이어는 탈락합니다.\n");
	printf("                             이 게임의 딜러는 go를 하여도 카드를 받지 않습니다.\n");
	printf("===========================================================================================================\n");
	printf("블랙잭 게임을 시작합니다.\n\n");
	//카드트레이 배열에 순서대로 0부터 51까지 번호가 들어가게  됨
	for(j=0;j<N_CARDSET*N_CARD;j++)
	{
		CardTray[j]=j+1;	
	}


	srand((unsigned)time(NULL));
	
	//플레이어 수 결정 
	configUser();

	//카드를 랜덤으로 섞음 
	mixCardTray();


	//모든 유저들에게 초기 배팅금 50달러씩 지급	
	for(j=0;j<n_user;j++)
	{
		dollar[j] = 50;
	}
	
	//Game start --------
	do {
		
		
		printf("\n\n\n------------------ Round %d (cardIndex : %d)--------------------------\n", roundIndex+1, cardIndex);
		
		//나 빼고 나머지 플레이어들은 보유 금액 미만의 돈을 걸음 
		for(j=1;j<n_user;j++) 
		{
			bet[j]=rand()%dollar[j]+1;
		} 
		
		//내 배팅금 결정  
		betDollar();
		
		//처음에 카드 2장씩 제공 
		offerCards(); 
		
		//카드가 전부 소진되면 게임 끝 
		if(cardIndex > 52)
		{
			//gameEnd = 0;
			break;
		}
		
		printf("\n\n-----------------GAME START------------------------\n");
		
		//처음 카드 상태 2장씩 보유하고 있는 것 출력 
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
				//카드 개수가 바닥나면 게임 종료 
				if (cardIndex>52)
					break;
				//유저 별 들고있는 카드 합 계산 	 
				for(m=0;m<numb[i];m++)
				{
					sum+=getCardNum(cardhold[i][m]);
				}
				cardSum[i] =sum;

				//sum을 0으로 해줘야 sum값에 그 전의 값이 더해지지 않음. 초기화 단계 
				sum=0;
				
				printUserCardStatus(i,numb[i]);
				
				//getAction에서 턴을 종료할건지를 값을 받아 정함 
				//getAction의 값을 반환, YOU가 stop을 외치거나 플레이어가 가진 카드의 합이 17이 넘으면action ==-1 
				action=getAction(i);
				if (action == -1)
				{
					break;
				}
				 
				calcStepResult(i);
				
				//배팅금이 0이 되면 게임이 아예 종료 
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

