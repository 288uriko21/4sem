#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

#define MAX_USERNAME 30 // максимальная длина имени клиента
#define BUFSIZE 1024    // размер буфера
#define MAX_CLIENT 8

char users[MAX_CLIENT][MAX_USERNAME] = {0};
int clients = 0;

int IfEnter(char *s)
{
  if (strchr(s, ':') == NULL)
  {
    return 1;
  }
  return 0;
}

/*функция распознающая \quit*/
int IfQuit(char *s)
{
  int i = 0;
  char *str;
  if ((str = strchr(s, ':')) == NULL)
    return 0;
  if ((str + 1) == strstr(s, "\\quit "))
    return 1;
  return 0;
}

int Ifhelp(char *s)
{
  int i = 0;
  char *str;
  if ((str = strchr(s, ':')) == NULL)
    return 0;
  if ((str + 1) == strstr(s, "\\help"))
    return 1;
  return 0;
}

int IfPrivate(char *s)
{
  int i = 0;
  char *str;
  if ((str = strchr(s, ':')) == NULL)
    return 0;
  if ((str + 1) == strstr(s, "\\private "))
    return 1;
  return 0;
}

int IfPrivates(char *s)
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
  char usname[MAX_USERNAME]; /*буфер для считывания имени*/
  char buf[BUFSIZE];
  char privates[MAX_CLIENT][MAX_USERNAME];
  int numbprivates = 0;

  for (int i = 0; i < MAX_CLIENT; ++i)
    privates[i][0] = '\0';

  if (argc < 2)
  {
    fprintf(stderr, "Необходимо указать номер порта\n");
    return 1;
  }

  sock = socket(AF_INET, SOCK_STREAM, 0);

  if (sock < 0)
  {
    perror("socket\n");
    exit(1);
  }

  sscanf(argv[1], "%d", &port);                  /*считываю номер порта*/
  addr.sin_family = AF_INET;                     /*выбор домена(для взаимодействия в рамках сети)[указываю через структуру AF_INET]*/
  addr.sin_port = htons(port);                   /*выбор порта(hton+ переводят данные из узлового порядка расположения байтов в сетевой и наоборот)*/
  addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK); /*выбор ip-адреса(все интерфейсы)*/

  if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
  { /*подключение к порту*/
    perror("connect\n");
    exit(2);
  }

  strcpy(buf, "usname:\\usershelp"); ////!!!!!!!!!!!!!!!!!!!!!!
  send(sock, buf, BUFSIZE, 0);
  recv(sock, buf, BUFSIZE, 0);
  /// сделать из полученного список имен
  i = 1;
  char usnameH[30];
  int uc = 0;
  while (i < BUFSIZE)
  {
    // if (buf[i] != '\0')
    //   break;
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
    }
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
  //scanf("%s", usname);
  i = 0;
  while (i < MAX_CLIENT)
  {
    if (strcmp(users[i], usname) == 0)
    {
      fprintf(stderr, "This name is already in use\n");
      fprintf(stderr, "Enter username:");
      scanf("%s", usname);
    }
    //fprintf(stderr, "%s\n", users[i]);
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
    /*записываем имя в буфер*/
    while (usname[namelen] != '\0')
    {
      buf[namelen] = usname[namelen];
      namelen++;
    }
    /*ставим разделительный символ : */
    buf[namelen++] = ':';
    //getchar();
    // printf("%s %d\n", buf, namelen);

    while (1)
    {
      /*считываем сообщение*/
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
      if (IfPrivates(buf))
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

      if (Ifhelp(buf))
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
        if (IfPrivate(buf))
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
            // printf("%d ", numbprivates);
          }
        }

        send(sock, buf, strlen(buf) + 1, 0);
        if (IfQuit(buf))
        {
          kill(getppid(), SIGKILL);
          break;
        }
      }
    }

    close(sock);
    _exit(0);

  default:

    while (1)
    {
      recv(sock, buf, BUFSIZE, 0);
      if (IfPrivate(buf))
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
      if (buf[0] != '!')
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
