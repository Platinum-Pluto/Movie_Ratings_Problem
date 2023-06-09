
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>
#include<string.h>

typedef struct {
    char name[256];
    char director[256];
    int release_date;
    float popularity;
} movie_t;
movie_t movie_database[100];
int num_movies;
int thread_amount = 100;
int thread_count = 0;
int thread_count1 = 0;
sem_t key, in;
pthread_mutex_t lock1;
int count = 0, n;
void *search(void *title)
{
    sem_wait(&in);
    printf("\nProcess %d reached\n", (int)pthread_self());
    sleep(1);
    printf("\n");
    pthread_mutex_lock(&lock1);
    if (count >= 5)
    {
        printf("\nProcess %d is waiting to search\n", (int)pthread_self());
    }
    count++;
    pthread_mutex_unlock(&lock1);
    sem_wait(&key);
    printf("\nProcess %d has started searching\n", (int)pthread_self());
    sleep(1);
    printf("\nProcess %d searching for %s\n", (int)pthread_self(), (char*)title);
    info((char*)title, (int)pthread_self());
    printf("\nProcess %d finished it's search\n\n", (int)pthread_self());
    sem_post(&key);
}

void sort_movies_by_rating() {
    for (int i = 0; i < num_movies - 1; i++) {
        for (int j = i + 1; j < num_movies; j++) {
            if (movie_database[j].popularity > movie_database[i].popularity) {
                movie_t temp = movie_database[i];
                movie_database[i] = movie_database[j];
                movie_database[j] = temp;
            }
        }
    }
}


int main()
{
    FILE* fp = fopen("Database.txt", "r");
    if (fp == NULL) {
        fprintf(stderr, "Error: unable to open movie database file\n");
        return 1;
    }
    num_movies = 0;
    while (fscanf(fp, "%[^,],%[^,],%d,%f", movie_database[num_movies].name,
                  movie_database[num_movies].director, &movie_database[num_movies].release_date,
                  &movie_database[num_movies].popularity) == 4) {
        num_movies++;
    }
        sort_movies_by_rating();

        int choice ;
        while(1)
        {
                count = 0;
                printf("\n\t\t\t\t............Menu..............\n");
                printf("\n1.Choose number of searching process \n2.Exit\n");
                printf("\nEnter choice: ");
                scanf("%d",&choice);
                if(choice==1)
                {
       printf("\nEnter the number of searching process : ");
    scanf("%d",&n);
    printf("\n");
    int i,j,c=0;
    sem_init(&key,0,5);
    sem_init(&in,0,0);
    pthread_mutex_init(&lock1,0);
    pthread_t movie[thread_amount];
     char m[thread_amount][100];
       printf("Enter Keyword -> \n");
    for(i=1; i<=n; i++)
        {
        printf("%d: ",i);
        scanf("%s",m[thread_count]);
        pthread_create(&movie[thread_count],NULL,search,(void*)&m[thread_count]);
	  thread_count++;
        getchar();

    if(i==n)
        {
        for(j=0; j<n; j++)
        sem_post(&in);
    }
    }
    printf("\n");
    for(i=0; i<n; i++)
        {
        pthread_join(movie[thread_count1],NULL);
        thread_count1++;
    }
    printf("All threads finished its' searching \n");
}
        else
        {
                exit(0);
        }
}
}
void info(char ch[],int no){
   int found = 0;
   int counter = 0;
      printf("Searching for movies with keyword '%s':\n", ch);
    for (int i = 0; i < num_movies; i++) {
        if (strstr(movie_database[i].name, ch) != NULL) {
            printf("  %s, directed by %s, released on %d (popularity rating: %.1f)\n ",
                   movie_database[i].name, movie_database[i].director, movie_database[i].release_date,
                   movie_database[i].popularity);
            found = 1;
            counter++;
        }
    }
    if (!found) {
        printf("  No movies found with keyword '%s'\n", ch);
    }
     else{
        printf("\n%d movies found with keyword %s\n", counter, ch);
    }
}


