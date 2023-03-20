#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BUFSIZE 1024
#define MAX_CLIENT 8
#define MAX_USERNAME 30

char users[MAX_CLIENT][MAX_USERNAME] = {0};
int clients = 0;

int recogHelp(char *s)
{
  int i = 0;
  char *str;
  if ((str = strchr(s, ':')) == NULL)
    return 0;
  if ((str + 1) == strstr(s, "\\help"))
    return 1;
  return 0;
}

int recogQuit(char *s)
{
  int i = 0;
  char *str;
  if ((str = strchr(s, ':')) == NULL)
    return 0;
  if ((str + 1) == strstr(s, "\\quit "))
    return 1;
  return 0;
}


int recogPrivate(char *s)
{
  int i = 0;
  char *str;
  if ((str = strchr(s, ':')) == NULL)
    return 0;
  if ((str + 1) == strstr(s, "\\private"))
    return 1;
  return 0;
}

int recogPrivates(char *s)
{
  int i = 0;
  char *str;
  if ((str = strchr(s, ':')) == NULL)
    return 0;
  if ((str + 1) == strstr(s, "\\privates"))
    return 1;
  return 0;
}

int main(int argc, char **argv)
{
  int sock, port, c, i, j, length, pid;
  struct sockaddr_in addr;
  char usname[MAX_USERNAME];
  char buf[BUFSIZE];
  char privates[MAX_CLIENT][MAX_USERNAME] = {0};
  int numbprivates = 0;

  for (int i = 0; i < MAX_CLIENT; ++i)
    privates[i][0] = '\0';

  if (argc < 2)
  {
    fprintf(stderr, "You must specify the port number in the parameters\n");
    return 1;
  }

  sock = socket(AF_INET, SOCK_STREAM, 0);

  if (sock < 0)
  {
    perror("socket\n");
    exit(1);
  }

  sscanf(argv[1], "%d", &port);
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

  if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
  { /*подключение к порту*/
    perror("connect\n");
    exit(2);
  }

  strcpy(buf, "usname:\\usershelp"); ////!!!!!!!!!!!!!!!!!!!!!!
  send(sock, buf, BUFSIZE, 0);
  recv(sock, buf, BUFSIZE, 0);

  i = 1;
  char usnameH[30];
  int uc = 0;
  //printf("%s opo\n", buf);
  while (i < BUFSIZE)
  {
    if (buf[i] == '\0')
      break;

    if (buf[i] != ' ')
    {
      usnameH[uc] = buf[i];
      ++uc;
    }
    else
    {
      usnameH[uc] = '\0';
      uc = 0;
      strcpy(users[clients++], usnameH);
     //printf("%s !\n", users[clients-1]);
    }
    //printf("%c -\n", buf[i]);
    ++i;
  }

  i = 0;
  char cimb;
  while (i == 0)
  {
    fprintf(stderr, "Enter username:");
    scanf("%c", &cimb);
    i = 0;
    while (cimb != '\n')
    {
      usname[i] = cimb;
      scanf("%c", &cimb);
      ++i;
    }
    if (i > 29)
    {
      i = 0;
      fprintf(stderr, "Too long name :(\n");
    }
  }
  usname[i] = '\0';
  // scanf("%s", usname);
  i = 0;
  while (i < MAX_CLIENT)
  {
    if (strcmp(users[i], usname) == 0)
    {
      fprintf(stderr, "This name is already in use\n");
      fprintf(stderr, "Enter username:");
      scanf("%s", usname);
    }
    // fprintf(stderr, "%s\n", users[i]);
    ++i;
  }

  for (i = 0; i < 3; i++)
    buf[i] = '*';
  for (i = 0; i < strlen(usname) + 1; i++)
    buf[i + 3] = usname[i];
  strcat(buf, " join the chat\0");
  length = strlen(buf) + 1;
  send(sock, buf, length, 0);

  switch (pid = fork())
  {
  case -1:
    perror("fork\n");
    close(sock);
    return 1;
  case 0:
    int namelen = 0;

    while (usname[namelen] != '\0')
    {
      buf[namelen] = usname[namelen];
      namelen++;
    }

    buf[namelen++] = ':';
    // getchar();
    //  printf("%s %d\n", buf, namelen);

    while (1)
    {

      j = namelen;
      char name[MAX_USERNAME];
      while (((c = getchar()) != '\n') && (j < BUFSIZE - 1))
      {
        buf[j++] = c;
      }
      if (j == BUFSIZE - 1)
      {
        fprintf(stderr, "Too long message, try again\n");
        strcat(usname, ":\\quit lost connection\0");
        send(sock, usname, strlen(usname) + 1, 0);
        kill(getppid(), SIGKILL);
        _exit(3);
      }
      buf[j] = '\0';
      if (recogPrivates(buf))
      {
        i = 0;
        while (privates[i][0] != '\0' && i < MAX_CLIENT)
        {
          fprintf(stderr, "You have sent private messages to users: ");
          fprintf(stderr, "%s ", privates[i]);
          ++i;
        }
        fprintf(stderr, "\n");
      }
      else
      {

        if (recogHelp(buf))
        {
          //////////////////////////////////////////////////////////////////////
          fprintf(stderr, "\\users -show users online\n");
          fprintf(stderr, "\\private <username> <message> - send private message to username\n");
          fprintf(stderr, "\\quit - exit\n");
          fprintf(stderr, "\\privates -show users, you sended private messages\n");
          fprintf(stderr, "\\help -show command hints\n");
        }

        else
        {
          if (recogPrivate(buf))
          {
            char name[MAX_USERNAME];
            i = 0;

            while (buf[i] != '<')
              ++i;
            ++i;
            int k = 0;
            while (buf[i] != '>')
            {
              name[k] = buf[i];
              ++i;
              ++k;
            }
            name[k] = '\0';

            int flag = 1;
            for (int i = 0; i < MAX_CLIENT; ++i)
              if (strcmp(privates[i], name) == 0)
              {
                flag = 0;
                break;
              }
            if (flag == 1)
            {
              strcpy(privates[numbprivates], name);
              ++numbprivates;
            }
          }

          send(sock, buf, strlen(buf) + 1, 0);
          if (recogQuit(buf))
          {
            kill(getppid(), SIGKILL);
            break;
          }
        }
      }
    }

    close(sock);
    _exit(0);

  default:

    while (1)
    {
      recv(sock, buf, BUFSIZE, 0);
      if (recogPrivate(buf))
      {
        char name[MAX_USERNAME];
        char from[MAX_USERNAME];
        int i = 0;
        while (buf[i] != ':')
        {
          from[i] = buf[i];
          ++i;
        }
        from[i] = '\0';
        strcat(from, "(private): ");
        while (buf[i] != '<')
          ++i;
        ++i;
        int k = 0;
        while (buf[i] != '>')
        {
          name[k] = buf[i];
          ++i;
          ++k;
        }
        name[k] = '\0';
        while (buf[i] != '<')
          ++i;
        ++i;
        if (strcmp(name, usname) == 0)
        {
          fprintf(stderr, "%s", from);
          while (buf[i] != '>')
          {
            fprintf(stderr, "%c", buf[i]);
            ++i;
          }
          fprintf(stderr, "%c", '\n');
        }
      }
      else if (buf[0] != '!')
      {
        fprintf(stderr, "%s\n", buf);
        if (strcmp(buf, "###server is shouting down, thanks to everyone") == 0)
        {
          kill(pid, SIGKILL);
          break;
        }
      }
    }

    close(sock);
  }

  return 0;
}
