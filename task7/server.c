#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#define BUFSIZE 1024   
#define QUEUE_SIZE 5    
#define MAX_CLIENT 8    
#define MAX_USERNAME 30 
int fd[2], clients = 0, socket1, fd2[2];
char users[MAX_CLIENT][MAX_USERNAME] = {0}; 


void SonHandler(int s)
{
  char bbuf[BUFSIZE];
  read(fd[0], bbuf, BUFSIZE);
  send(socket1, bbuf, BUFSIZE, 0);
  /*отсылаю в сокет*/
  return;
}

// void handlername(int s)
// {
//   char bbuf[BUFSIZE];
//   read(fd2[0], bbuf, BUFSIZE);
//   // fprintf(stderr,"Сын получил %s\n", bbuf);
//   send(socket1, bbuf, BUFSIZE, 0);
//   /*отсылаю в сокет*/
//   printf("!!!!!!!!!!!!!!%c\n", bbuf[2]);

//   if (bbuf[2] == 'n')
//   {
//     recv(socket1, bbuf, BUFSIZE, 0);
//     printf("NONONO %s\n", bbuf);
//     write(fd[1], bbuf, BUFSIZE);
//   }
//   return;
// }


int recogUsers(char *s)
{
  char *str;
  if ((str = strchr(s, ':')) == NULL)
    return 0;
  if (strcmp(str + 1, "\\users") == 0)
    return 1;
  return 0;
}

int recogUsershelp(char *s)
{
  char *str;
  if ((str = strchr(s, ':')) == NULL)
    return 0;
  if (strcmp(str + 1, "\\usershelp") == 0)
    return 1;
  return 0;
}

void StrNull(char *s)
{
  int i;
  for (i = 0; i < MAX_USERNAME; i++)
    s[i] = '\0';
  return;
}


int recogQuit(char *s)
{
  int i = 0;
  char *str;
  if ((str = strchr(s, ':')) == NULL)
    return 0;
  if ((str + 1) == strstr(s, "\\quit"))
    return 1;
  return 0;
}

int recogEnter(char *s)
{
  if (strchr(s, ':') == NULL)
  {
    return 1;
  }
  return 0;
}

int IfPrivate(char *s)
{
  char *str = strchr(s, ':');
  if ((str + 1) == strstr(s, "\\private"))
    return 1;
  return 0;
}

/////////////////////////////////////////////////////////////////////////


void PHandler(int s)
{
  char bbuf[BUFSIZE];
  int j, length, i = 0;
  char usname[BUFSIZE] = {0};
  read(fd[0], bbuf, BUFSIZE);
  

  if (recogQuit(bbuf))
  {

    clients--;
    for (j = 0; j < 3; j++)
      usname[j] = '*';
    j = 0;
    while (bbuf[j] != ':')
    {
      usname[j + 3] = bbuf[j];
      j++;
    }
    // usname[i] = '\0';
    while (i < MAX_CLIENT)
    {
      if (strcmp(users[i], usname + 3) == 0)
      {
        StrNull(users[i]);
        break;
      }
      i++;
    }


    length = strlen("\\quit") + 2;
    strcat(usname, " leave the chat with message:"); 
    strcat(usname, bbuf + j + length);
 
    for (j = 0; j < clients; j++)
    {
      write(fd[1], usname, BUFSIZE);
    }
  }

  else

      if (recogEnter(bbuf))
  {
    while (bbuf[i] != ' ')
    {
      if (i >= 3)
        usname[i - 3] = bbuf[i];
      i++;
    }
    i = 0;
    while (i < MAX_CLIENT)
    {
      if (strlen(users[i]) == 0)
      {
        strcpy(users[i], usname);
        break;
      }
      i++;
    }
    for (j = 0; j < clients; j++)
      write(fd[1], bbuf, BUFSIZE);

    // while (bbuf[i] != ' ')
    // {
    //   if (i >= 3)
    //     usname[i - 3] = bbuf[i];
    //   i++;
    // }
    // i = 0;
    // while (i < MAX_CLIENT)
    // {
    //   if (strcmp(users[i], usname) == 0)
    //   {
    //     char info[BUFSIZE] = "!!no";
    //     write(fd2[1], info, BUFSIZE);
    //     kill(0, SIGALRM);
    //     read(fd[0], bbuf, BUFSIZE);
    //     printf("%s name", bbuf);
    //     i = 0;
    //     while (bbuf[i] != ' ')
    //     {
    //       if (i >= 3)
    //         usname[i - 3] = bbuf[i];
    //       i++;
    //     }
    //     i = 0;
    //   }
    //   if (strlen(users[i]) == 0)
    //   {
    //     strcpy(users[i], usname);
    //     char info[BUFSIZE] = "!!y";
    //     write(fd2[1], info, BUFSIZE);
    //     kill(0, SIGALRM);
    //     for (j = 0; j < clients; j++)
    //       write(fd[1], bbuf, BUFSIZE);
    //     break;
    //   }
    //   i++;
    // }

  }

  else

      if (recogUsers(bbuf))
  {

    strcpy(bbuf, "users online: ");
    while (i < MAX_CLIENT)
    {
      if (strlen(users[i]) > 0)
      {
        strcat(bbuf, users[i]);
        strcat(bbuf, " ");
      }
      i++;
    }

    for (j = 0; j < clients; j++)
      write(fd[1], bbuf, BUFSIZE);
  }
  else
  {
    if (recogUsershelp(bbuf))
    {
      strcpy(bbuf, "!");
      while (i < MAX_CLIENT)
      {
        if (strlen(users[i]) > 0)
        {
          strcat(bbuf, users[i]);
          strcat(bbuf, " ");
        }
        i++;
      }
      
      for (j = 0; j < clients; j++)
        write(fd[1], bbuf, BUFSIZE);
    }
    else
    {
      for (j = 0; j < clients; j++)
        write(fd[1], bbuf, BUFSIZE);
    }
  }
  kill(0, SIGUSR2);
  return;
}

/////////////////////////////////////////////////////////////////////////

void SigintHandler(int s)
{
  char bbuf[] = "###server is shouting down, thanks to everyone\0";
  send(socket1, bbuf, strlen(bbuf) + 1, 0); 
  _exit(0);
}

int main(int argc, char **argv)
{
  int sock, listener, port;
  struct sockaddr_in addr;
  char buf[BUFSIZE];

  if (argc < 2)
  {
    fprintf(stderr, "You must specify the port number in the parameters\n");
    return 1;
  }

  listener = socket(AF_INET, SOCK_STREAM, 0); 
  if (listener < 0)
  {
    perror("socket");
    exit(1);
  }

  sscanf(argv[1], "%d", &port);             
  addr.sin_family = AF_INET;                
  addr.sin_port = htons(port);          
  addr.sin_addr.s_addr = htonl(INADDR_ANY); 

  if (bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0)
  {
    perror("bind");
    exit(2);
  }

  listen(listener, QUEUE_SIZE); 
  fprintf(stderr, "%s", "server start\n");

  pipe(fd);
  pipe(fd2);


  while (1)
  {

    if (clients < MAX_CLIENT)
    {
      sock = accept(listener, NULL, NULL); 
      if (sock < 0)
      {
        perror("accept error");
        exit(3);
      }
      clients++;
    }

    else
    {
      continue;
    }

    switch (fork())
    {

    case -1: 
      perror("fork");
      break;

    case 0:          
      close(listener); 
      socket1 = sock;
      signal(SIGUSR2, SonHandler);     
      signal(SIGINT, SigintHandler);
      signal(SIGUSR1, SIG_IGN);
      // signal(SIGALRM, handlername);

      while (1)
      {
        recv(socket1, buf, BUFSIZE, 0); 
        fprintf(stderr, "GET:%s\n", buf);
        write(fd[1], buf, BUFSIZE); 

        if (IfQuit(buf))
        { 
          kill(getppid(), SIGUSR1);
          break;
        }

        kill(getppid(), SIGUSR1);
      }


    default: 
      signal(SIGUSR2, SIG_IGN);
      signal(SIGUSR1, PHandler);
    }
  }
  close(listener);
  return 0;
}