#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_SONGS_PER_USER   10
#define MAX_USERS    		  5
#define MAX_ARTISTS		    100


#define NUM_TEST_USERS 		7


// This struct represents a song in the system
typedef struct {
	char      *title;
	char      *artist;
	short int  duration;
	short int  royalties;   
} Song;

// This struct represents a user in the system
typedef struct {
	char       *name;
	char        online; 				   // 1 = YES, 0 = NO
	Song       *songs[MAX_SONGS_PER_USER]; // songs this user has
	short int   numSongs;  				   // number of songs this user has
} User;

// This structure represents the Tune Share system
typedef struct {
	User  		*users[MAX_USERS];     // An array of all registered users
	short int	 numUsers;
} TuneShare;

/*
  Function:  registerUser
  Purpose:   Registers a user with the given char *name to the given TuneShare *t if the given TuneShare != MAX_USERS. Allocates memory for the user and initializes all the values.
       in:   The TuneShare for which the user will be registered to and the name of the user.
       out:  Function returns 0 on failure to add (tuneshare is at max capacity or the user is already registered.
       out:  Function returns 1 on succesful register.
*/

int registerUser(TuneShare *t,char *name){

	int x = t->numUsers;
	int k=0;
	
	
	
	if(t->numUsers == MAX_USERS){
		return 0;
	}
	else{
		for(int i = 0;i<t->numUsers;i++){
			if(t->users[i]->name == name){
				return 0;
			}	
		}
	
		User *newUser = (User*)malloc(1*sizeof(User));
		newUser->name = name;
		strcpy(&newUser->online,"0");
		newUser->numSongs=0;
		t->users[t->numUsers]=newUser;
		t->numUsers++;	
		return 1;
	}
	

	
	
}
/*
  Function:  addSong
   Purpose:  Adds a song to the given user's array of songs as long as they dont have MAX_SONGS_PER_USER. Allocates memory for the song and initializes all values
        in:  The user which will have the song added to as well as the title, artist, and length of the song in seconds. 
       out:  Function returns 0 when the user has max songs
       out:  Function returns 1 when song is successfully added
*/
int addSong(User *u,char *title,char *artist,short int duration){
	if(u->numSongs == MAX_SONGS_PER_USER){
		return 0;
	}
	else{
		Song *newSong = (Song*)malloc(sizeof(Song*));
		newSong->title = title;
		newSong->artist = artist;
		newSong->duration = duration;
		newSong->royalties = 0;
		u->songs[u->numSongs]=newSong;
		u->numSongs += 1;
		return 1;
	}
}
/*
  Function: userWithName 
   Purpose: To find and return a pointer to the user with the given name 
        in: The TuneShare in which to search and the name of the user to return
       out: Function returns NULL if the user is not found
       out: Function returns a user pointer to the user in the tuneshare with the given name. 
*/
User *userWithName(TuneShare *t,char *name){
	for(int i=0;i<t->numUsers;i++){
		if(strcmp(t->users[i]->name,name)==0){
			return t->users[i];
		}
	}	
	return NULL;		
}
/*
  Function:  getSong
   Purpose:  To find and return a pointer to the song with the given title from the given user
        in:  The tuneshare as well as the title and name of person to search. 
       out:  Function returns NULL when the given user does not exist, is offline, or the song is not found
       out:  Function returns a song pointer to the song found in the given user's songlist
*/
Song *getSong(TuneShare *t,char *title, char *ownerName){
	if(userWithName(t,ownerName)==NULL){
		return NULL;
	}
	if(userWithName(t,ownerName)->online == '0'){
		return NULL;
	}
	else{
		for(int i=0;i<userWithName(t,ownerName)->numSongs;i++){
			if(userWithName(t,ownerName)->songs[i]->title == title){
				return userWithName(t,ownerName)->songs[i];
			}
		}
		return NULL;
	}
}
/*
  Function: downloadSong 
   Purpose:  To enable a user to add a song to their library if it exists in another person's library
        in:   The tuneshare as well as the person sharing the song, person downloading the song, and the title of the song.
       out:  As long as both user's exist, the song exists, and they are both online, the song will be created and have memory allocated for it. The details of the song will be taken from the song to be downloaded
       out:  
*/
void downloadSong(TuneShare *t, char *downloadToName, char *title, char *downloadFromName){

	Song *downSong = getSong(t,title,downloadFromName);
	User *fromUser = userWithName(t,downloadFromName);
	User *toUser = userWithName(t,downloadToName);
	if(toUser == NULL){
		return;
	}
	if(toUser->numSongs==MAX_SONGS_PER_USER){
		return;
	}
	if(fromUser==NULL){
		return;
	}
	if(fromUser->online == '0'){
		return;
	}
	if(downSong==NULL){
		return;
	}
	

	Song *newSong = (Song*)malloc(sizeof(Song*));
	newSong->title = downSong->title;
	newSong->artist = downSong->artist;
	newSong->duration = downSong->duration;
	
	downSong->royalties++;
	toUser->songs[toUser->numSongs]=newSong;
	toUser->numSongs += 1;

}
/*
  Function:  calcRoyal
   Purpose:  calculates the amount of times that a given artist has had one of their songs downloaded
        in:  The tuneshare as well as the artist in which the royalties will be counted. 
       out:  Function returns the amount of times an artist has had their song downloaded
       out:  
*/
int calcRoyal(TuneShare *t, char *artist){

	int amount=0;

	for(int i=0;i<t->numUsers;i++){
		for(int k=0;k<t->users[i]->numSongs;k++){
			if(strcmp(t->users[i]->songs[k]->artist,artist)==0){
				amount+=t->users[i]->songs[k]->royalties;
			}
		}
	}
	
	return amount;

}
/*
  Function:  displayRoyalties
   Purpose:  To print the royalties for the artists in a neat and formatted way
        in:  The tuneshare which the artists are located 
       out:  
       out:  
*/
void displayRoyalties(TuneShare *t){
		
	printf("%.2f	Neil Diamond\n",(float)calcRoyal(t,"Neil Diamond")*0.25);
	printf("%.2f	Ryland James\n",(float)calcRoyal(t,"Ryland James")*0.25);
	printf("%.2f	E-Type\n",(float)calcRoyal(t,"E-Type")*0.25);
	printf("%.2f	Hollerado\n",(float)calcRoyal(t,"Hollerado")*0.25);
	printf("%.2f	Divine Brown\n",(float)calcRoyal(t,"Divine Brown")*0.25);
	
}

/*
  Function:  logon
   Purpose:  To set the users online status to "1" meaning YES as long as the user exists and isnt already online.
        in:   The tuneshare as well as the name of the user to be set as "online"
       out:  
       out:  
*/
void logon(TuneShare *t,char *name){
	if(userWithName(t,name)==NULL){
		return;
	}
	if(&userWithName(t,name)->online == "1"){
		return;
	}
	else{
		strcpy(&userWithName(t,name)->online,"1");
	}
}
/*
  Function:  logon
   Purpose:  To set the users online status to "0" meaning NO as long as the user exist and isnt already offline.
        in:   The tuneshare as well as the name of the user to be set as "offline"
       out:  
       out:  
*/
void logoff(TuneShare *t,char *name){
	if(userWithName(t,name)==NULL){
		return;
	}
	if(&userWithName(t,name)->online == "0"){
		return;
	}
	else{
		strcpy(&userWithName(t,name)->online,"0");
	}
}
/*
  Function:  onlineUsers
   Purpose:  To retrieve a pointer to an array of all users which are ONLINE
        in:   The tuneshare as well as a variable to be updated with the number of online users
       out:  Function returns a pointer to an array of users, all of which are online at the time;
       out:  Function also updates the numOnLine variable so that it can be accessed from outside the function
*/
User** onlineUsers(TuneShare *t,short int *numOnLine){
	
	*numOnLine =0;
	
	
	for(int i = 0;i< t->numUsers;i++){
		if(t->users[i]->online == '1'){
			(*numOnLine)++;		
		}
	}
	
	User** onlineUsersArr = (User**)malloc((*numOnLine)*(sizeof(User*)));
	
	int k =0;
	
	for(int i = 0;i< t->numUsers;i++){
		if(t->users[i]->online == '1'){	
			onlineUsersArr[k] = t->users[i];
			k++;	
		}
	}

	
	return onlineUsersArr;
	
}
/*
  Function:  allAvailableSongs
   Purpose:  To retrieve a pointer to an array of all the songs from users which are ONLINE
        in:   The tuneshare as well as a variable to be updated with the number of available songs
       out:  Function returns a pointer to an array of songs, all of which came from users that are online
       out:  Function also updates the numSongs variable so that it can be accessed from outside the function.
*/
Song** allAvailableSongs(TuneShare *t,short int *numSongs){
	

	
	*numSongs =0;
	
	for(int i = 0;i< t->numUsers;i++){
		if(t->users[i]->online == '1'){
			for(int j=0;j<t->users[i]->numSongs;j++){
				(*numSongs)++;
			}
		}
	}
	
	Song**availSongs = (Song**) malloc((*numSongs)*sizeof(Song*));
	
	int r=0;

	for(int i = 0;i< t->numUsers;i++){
		if(t->users[i]->online == '1'){
			for(int x =0; x<t->users[i]->numSongs;x++){
				availSongs[r] = t->users[i]->songs[x];
				r++;
			}
		}
	}
	
	return availSongs;
	
}
/*
  Function:  displayStats
   Purpose:  To display the online users, the songs associated with each user, and all the songs from every online user
        in:   The tuneshare which you would like to display the stats of
       out:  Function neatly prints formatted text to the screen, using the onlineUsers and allAvailableSongs function to retrieve the pertinent data each time.
       out:  
*/
void displayStats(TuneShare *t){
	printf("\nTune Share Center (%d registered users)\n\n",t->numUsers);
	
	short int numonline = 0;
	
	short int numsongs = 0;
	
	User** ptr = onlineUsers(t,&numonline);
	Song** ptrSong = allAvailableSongs(t,&numsongs);
	
	
	if(numonline==0 && numsongs==0){
		printf("%d Online Users:\n	NONE\n\n%d Available Songs:\n	NONE\n",numonline,numsongs);
	}
	else{
		
		printf("%d Online Users:\n",numonline);
		for(int x=0;x<numonline;x++){
			printf("	%s with %d songs as follows:\n",ptr[x]->name,ptr[x]->numSongs);
			for(int y=0;y<ptr[x]->numSongs;y++){
				printf("		%s by %s\n",ptr[x]->songs[y]->title, ptr[x]->songs[y]->artist);
			}
			
		}
		printf("\n%d Available Songs:\n",numsongs);
			for(int q=0;q<numsongs;q++){
			
			printf("	%s by %s (%.2f)\n",ptrSong[q]->title,ptrSong[q]->artist,(float)ptrSong[q]->duration/60);
			
			}
	}

	free(ptr);
	free(ptrSong);
	
}
/*
  Function:  displayAvailableSongsByArtist
   Purpose:  To display a list of available songs by a given artist
        in:   The tuneshare as well as the name of the artist in which songs you would like to check
       out:  Function neatly prints formatted text to the screen, displaying the songs available by the given artist.
       out:  
*/
void displayAvailableSongsByArtist(TuneShare *t, char *artist){
	
	int songs=0;
	
	for(int i = 0;i< t->numUsers;i++){
		for(int x =0; x<t->users[i]->numSongs;x++){
			if(t->users[i]->songs[x]->artist == artist && t->users[i]->online == '1'){
				printf("	%s by %s\n",t->users[i]->songs[x]->title,t->users[i]->songs[x]->artist);
				songs++;
			}			
		}
	}
	
	if(songs==0){
		printf("	NONE\n");
	}
	

}
/*
  Function:  shutDown
   Purpose:  To set all users to offline and free all memory to prevent leaks
        in:   The tuneshare to shut down
       out:  Function goes through every song, freeing the memory
       out:  Function goes through every user, freeing the memory
*/
void shutDown(TuneShare *t){
	for(int i=0;i<t->numUsers;i++){
		
		for(int k=0;k<t->users[i]->numSongs;k++){
			free(t->users[i]->songs[k]);
		}
		
		strcpy(&t->users[i]->online, "0");
		free(t->users[i]);
	}
}

int main() {

	static char *TITLES[] = {"Back 2 Life", "Loneliness For Love", "Desire 126", "Perfect", "In My Head", 
							 "Old Skool Love", "Let's Go", "No Place", "We Gotta Go", "How You Get the Girl", 
							 "Catch", "Here Tonight", "Details", "Dangerous", "Brighter Than the Sun",
				  			 "Photograph", "Thinking Out Loud", "If Heaven Were to Fall", "I Just Wanna Be With You", 
				  			 "Song Sung Blue", "Outta Style", "Why", };
				  
	static char *ARTISTS[] = {"E-Type", "Lovely the Band", "Hollerado", "Ed Sheeran", "Ryland James", 
							  "Divine Brown", "Group 1 Crew", "Backstreet Boys", "E-Type", "Taylor Swift", 
							  "Brett Young", "Brett Young", "Billy Currington", "Kardinal Offichall", 
							  "Colbie Caillat", "Ed Sheeran", "Ed Sheeran", "E-Type", "E-Type", "Neil Diamond", 
				   			  "Aaron Watson", "Frankie Avalon", };
				   
	static int   DURATIONS[] = {217, 237, 187, 263, 205, 204, 256, 179, 213, 247, 196, 
								216, 201, 251, 231, 202, 281, 223, 230, 185, 222, 161};


	static char *TEST_USER_NAMES[NUM_TEST_USERS] = {"Disco Stew", "Peter Punk", "Country Candy", "Ronnie Rocker", 
													"Sleeping Sam", "Disco Stew", "Mellow Marvin"};

	static int  LIST_SIZES[NUM_TEST_USERS] = {7, 9, 9, 5, 1, 0, 0};
	static int  SONGNUMS[NUM_TEST_USERS][MAX_SONGS_PER_USER] = {
								   {1, 2, 4, 5, 12, 15, 21}, {0, 1, 3, 8, 9, 13, 14, 17, 20}, 
								   {6, 7, 8, 10, 11, 12, 13, 20, 21}, {0, 8, 16, 17, 18}, {19}, {}, {}};

	
	// Create the TuneShare Center
	TuneShare   tuneShareCenter;
	tuneShareCenter.numUsers = 0;
	
	// Attempt to register all test users
	for (int i=0; i<NUM_TEST_USERS; i++) {
		if (!registerUser(&tuneShareCenter, TEST_USER_NAMES[i]))
			printf("Error adding User: \"%s\"\n", TEST_USER_NAMES[i]);
		else
			printf("User: \"%s\" has been registered\n", TEST_USER_NAMES[i]);
	}
	


	// Display some stats
	displayStats(&tuneShareCenter);
	
	
		// Log on a user
	printf("\nLogging on a user...\n");
	logon(&tuneShareCenter, "Disco Stew");
	

	
	// Display some stats
	displayStats(&tuneShareCenter);
	
	
	
	// Now add all the test songs for these test users
	for (int i=0; i<tuneShareCenter.numUsers; i++) {
		for (int j=0; j<LIST_SIZES[i]; j++) 
			addSong(tuneShareCenter.users[i], TITLES[SONGNUMS[i][j]], ARTISTS[SONGNUMS[i][j]], DURATIONS[SONGNUMS[i][j]]);
	}

	// Display some stats
	displayStats(&tuneShareCenter);

	

	// Display all songs by E-Type
	printf("Available Songs By E-Type: \n");
	displayAvailableSongsByArtist(&tuneShareCenter, "E-Type");
	
	// Now bring two more users online
	printf("\nLogging on three users...\n");
	logon(&tuneShareCenter, "Disco Stew");    // Already logged on ... does nothing
	logon(&tuneShareCenter, "Mellow Marvin"); // Won't Work
	logon(&tuneShareCenter, "Peter Punk");
	logon(&tuneShareCenter, "Country Candy");

	

	// Display some stats again
	displayStats(&tuneShareCenter);
	
	
	// Log the last two users online
	printf("\nLogging on two more users...\n");
	logon(&tuneShareCenter, "Ronnie Rocker");
	logon(&tuneShareCenter, "Sleeping Sam");

	// Display some stats again
	displayStats(&tuneShareCenter);
	
	
	
	// Display all songs by E-Type
	printf("Available Songs By E-Type: \n");
	displayAvailableSongsByArtist(&tuneShareCenter, "E-Type");
		
	// Log off a couple of users
	printf("\nLogging off some users...\n");
	logoff(&tuneShareCenter, "Country Candy");
	logoff(&tuneShareCenter, "Mellow Marvin"); // Won't work
	logoff(&tuneShareCenter, "Peter Punk");

	// Display some stats again
	displayStats(&tuneShareCenter);
	
	// Display all songs by E-Type
	printf("Available Songs By E-Type: \n");
	displayAvailableSongsByArtist(&tuneShareCenter, "E-Type");
	
	
	

	// Have Sleeping Sam download some songs from other online users
	printf("\nSleeping Sam downloading 2 songs from Disco Stew and 2 from Ronnie Rocker...\n");
	downloadSong(&tuneShareCenter, "Sleeping Sam", "Old Skool Love", "Disco Stew");
	downloadSong(&tuneShareCenter, "Sleeping Sam", "In My Head", "Disco Stew");
	downloadSong(&tuneShareCenter, "Sleeping Sam", "If Heaven Were to Fall", "Ronnie Rocker");
	downloadSong(&tuneShareCenter, "Sleeping Sam", "I Just Wanna Be With You", "Ronnie Rocker");
	downloadSong(&tuneShareCenter, "Sleeping Sam", "We Gotta Go", "Country Candy"); 
	
	displayStats(&tuneShareCenter);
	

	

	// Have Ronnie Rocker download from Sleeping Sam and Disco Stew
	printf("\nRonnie Rocker downloading a songs from Sleeping Sam and a song from Disco Stew...\n");
	downloadSong(&tuneShareCenter, "Ronnie Rocker", "Song Sung Blue", "Sleeping Sam"); 
	downloadSong(&tuneShareCenter, "Ronnie Rocker", "Desire 126", "Disco Stew"); 
	
	
	//None of these should work
	downloadSong(&tuneShareCenter, "Sleeping Sam", "Back 2 Life", "Disco Stew");		// Won't work ... not a song of his
	downloadSong(&tuneShareCenter, "Sleeping Sam", "No Place", "Mellow Marvin"); 		// Won't work, not a user
	downloadSong(&tuneShareCenter, "Mellow Marvin", "Song Sung Blue", "Sleeping Sam"); 	// Won't work, not registered user
	downloadSong(&tuneShareCenter, "Ronnie Rocker", "Song Sung Blue", "Mellow Marvin"); // Won't work, not registered user
	downloadSong(&tuneShareCenter, "Country Candy", "Song Sung Blue", "Sleeping Sam"); 	// Won't work, not logged on
	downloadSong(&tuneShareCenter, "Sleeping Song", "Popsicles", "Ronnie Rocker"); 		// Won't work, song doesn't exist
	
	
	
	

	// Log on Country Candy and have her download a song from Ronnie
	printf("\nLogging on Country Candy...\n");
	logon(&tuneShareCenter, "Country Candy");
	downloadSong(&tuneShareCenter, "Country Candy", "Song Sung Blue", "Ronnie Rocker"); 

	// Display some stats again
	displayStats(&tuneShareCenter);

	// Log off the last few users
	printf("\nLogging off three more users...\n");
	logoff(&tuneShareCenter, "Disco Stew");
	logoff(&tuneShareCenter, "Ronnie Rocker");
	logoff(&tuneShareCenter, "Sleeping Sam");
	logoff(&tuneShareCenter, "Sleeping Sam"); // Won't Work now

	// Display some stats again
	displayStats(&tuneShareCenter);
	
	// Display all songs by E-Type
	printf("Available Songs By E-Type: \n");
	displayAvailableSongsByArtist(&tuneShareCenter, "E-Type");

	// Now display the royalties for all downloaded songs at 25 cents per song
	printf("\nHere are the royalties to be paid:\n");
	displayRoyalties(&tuneShareCenter);
	
	

	// Shut down the Tune Share center
	shutDown(&tuneShareCenter);
	
	
}
 
