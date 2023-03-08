#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#define BUFSIZE 1024    // размер буфера
#define QUEUE_SIZE 5    // максимальный размер очереди запросов
#define MAX_CLIENT 8    // нужно поменять на 8
#define MAX_USERNAME 30 // максимальная длина имени пользователя
int fd[2], clients = 0, socket1, fd2[2];
char users[MAX_CLIENT][MAX_USERNAME] = {0}; // массив для хранения имён клиентов

/*функция-обработчик для сына*/
void SHandler(int s)
{
  char bbuf[BUFSIZE];
  read(fd[0], bbuf, BUFSIZE);
  fprintf(stderr, "Сын получил %s\n", bbuf);
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

/*функция, распознающая запрос \users */
int IfUsers(char *s)
{
  char *str;
  if ((str = strchr(s, ':')) == NULL)
    return 0;
  if (strcmp(str + 1, "\\users") == 0)
    return 1;
  return 0;
}

int IfUsershelp(char *s)
{
  char *str;
  if ((str = strchr(s, ':')) == NULL)
    return 0;
  if (strcmp(str + 1, "\\usershelp") == 0)
    return 1;
  return 0;
}

/*функция, зануляющая строку с именем */
void StrNull(char *s)
{
  int i;
  for (i = 0; i < MAX_USERNAME; i++)
    s[i] = '\0';
  return;
}

void PrintUsers(void)
{
  int i;
  fprintf(stderr, "СПИСОК ПОЛЬЗОВАТЕЛЕЙ\n");
  for (i = 0; i < MAX_CLIENT; i++)
    if (strlen(users[i]) > 0)
      printf("%s\n", users[i]);
  return;
}

/*функция, распознающая \quit */
int IfQuit(char *s)
{
  int i = 0;
  char *str;
  if ((str = strchr(s, ':')) == NULL)
    return 0;
  if ((str + 1) == strstr(s, "\\quit"))
    return 1;
  return 0;
}

/*функция, распознающая входное сообщение*/
int IfEnter(char *s)
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

/*функция-обработчик для отца(выполнение quit, users, mssg)*/
void PHandler(int s)
{
  char bbuf[BUFSIZE];
  int j, length, i = 0;
  char usname[BUFSIZE] = {0};
  read(fd[0], bbuf, BUFSIZE);
  fprintf(stderr, "Отец получил %s\n", bbuf);

  if (IfQuit(bbuf))
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
    // PrintUsers();
    // fprintf(stderr, "Количество клиентов %d\n", clients);
    length = strlen("\\quit") + 2;
    strcat(usname, " leave the chat with message:"); /*объединение имя+выход*/
    strcat(usname, bbuf + j + length);
    // fprintf(stderr,"Будет отправлено %s\n",usname);
    for (j = 0; j < clients; j++)
    {
      write(fd[1], usname, BUFSIZE);
    }
  }

  else

      if (IfEnter(bbuf))
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
    // fprintf(stderr, "Количество клиентов %d\n", clients);
    // PrintUsers();
    // fprintf(stderr,"Будет отправлено %s\n",bbuf);
  }

  else

      if (IfUsers(bbuf))
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
    // fprintf(stderr,"Будет отправлено %s\n",bbuf);
    for (j = 0; j < clients; j++)
      write(fd[1], bbuf, BUFSIZE);
  }
  else
  {
    if (IfUsershelp(bbuf))
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
      fprintf(stderr, "Будет отправлено %s\n", bbuf);
      for (j = 0; j < clients; j++)
        write(fd[1], bbuf, BUFSIZE);
    }
  }
  kill(0, SIGUSR2);
  return;
}

/////////////////////////////////////////////////////////////////////////
/*функция-обработчик для завершения работы сервера*/
void SigintHandler(int s)
{
  char bbuf[] = "###server is shouting down, thanks to everyone\0";
  send(socket1, bbuf, strlen(bbuf) + 1, 0); /*отсылаю в сокет*/
  _exit(0);
}

int main(int argc, char **argv)
{
  int sock, listener, port;
  struct sockaddr_in addr;
  char buf[BUFSIZE];

  if (argc < 2)
  {
    fprintf(stderr, "Необходимо указать номер порта в параметрах\n");
    return 1;
  }

  listener = socket(AF_INET, SOCK_STREAM, 0); /*создаю сокет слушателя(дескриптор сокета)*/
  if (listener < 0)
  {
    perror("socket");
    exit(1);
  }

  sscanf(argv[1], "%d", &port);             /*считываю номер порта*/
  addr.sin_family = AF_INET;                /*выбор домена(для взаимодействия в рамках сети)[указываю через структуру AF_INET]*/
  addr.sin_port = htons(port);              /*выбор порта(hton+ переводят данные из узлового порядка расположения байтов в сетевой и наоборот)*/
  addr.sin_addr.s_addr = htonl(INADDR_ANY); /*выбор ip-адреса(все интерфейсы)*/

  if (bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0)
  { /*связываю сокет слушателя с портом*/
    perror("bind");
    exit(2);
  }

  listen(listener, QUEUE_SIZE); /*делаем сокет слушающим(сокетов готов принимать запросы(5 максимум))*/
  fprintf(stderr, "%s", "server start\n");

  pipe(fd);
  pipe(fd2);

  /*начинается обработка запросов на соединение*/

  while (1)
  {

    if (clients < MAX_CLIENT)
    {
      sock = accept(listener, NULL, NULL); /*возвращает дескриптор сокета взаимодействия; т.к. адрес клиента не интересует - NULL*/
      if (sock < 0)
      {
        perror("accept");
        exit(3);
      }
      clients++;
    }

    else
    {
      /*fprintf(stderr, "Server is full");*/
      continue;
    }

    switch (fork())
    {

    case -1: /*ошибка*/
      perror("fork");
      break;

    case 0:            /*сын(обрабатывает запрос и посылает ответ)*/
      close(listener); /*сокет сыну не нужен*/
      socket1 = sock;
      signal(SIGUSR2, SHandler);     /*обработчик для сына*/
      signal(SIGINT, SigintHandler); /*обработчик завершения работы серва*/
      signal(SIGUSR1, SIG_IGN);
      // signal(SIGALRM, handlername);

      while (1)
      {
        recv(socket1, buf, BUFSIZE, 0); /*получение данных из сокета*/
        fprintf(stderr, "Получил:%s\n", buf);
        write(fd[1], buf, BUFSIZE); /*запись этого в пайп*/

        if (IfQuit(buf))
        { /*при встрече \quit */
          kill(getppid(), SIGUSR1);
          break;
        }

        kill(getppid(), SIGUSR1);
      }

      _exit(0); /*завершение клиента с закрытием дескрипторов(chld завершены)*/

    default: /*отец*/
      signal(SIGUSR2, SIG_IGN);
      signal(SIGUSR1, PHandler);
      // signal(SIGALRM, SIG_IGN);
    }
  }
  close(listener);
  return 0;
}
