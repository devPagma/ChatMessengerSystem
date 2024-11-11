#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_USER 100

typedef struct Message{
char content[256];
char sender[50];
struct Message *prev;
struct Message *next;
} MESSAGE;

typedef struct Chat{
struct Message *head;
struct Message *tail;
} CHAT;

typedef struct User{
char useId[50];
char usedname[50];
CHAT *chatHistory;
} USER;

USER *userDirectory[MAX_USER];

void trimNewline(char *str){
size_t len = strlen(str);
if(len > 0 && str[len-1] == '\n'){
str[len-1] = '\0';
}
}

int hashFunction(char *userId){
int hash = 0;
for(int i = 0; userId[i] != '\0'; i++){
hash = (hash + userId[i]) % MAX_USER;
}
return hash;
}

void createUser(char *userId, char *userName){
trimNewline(userId);
int index = hashFunction(userId);

while(userDirectory[index] != NULL){
index = (index + 1) % MAX_USER;
}

USER *newUser = (USER *)malloc(sizeof(USER));
strcpy(newUser->useId, userId);
strcpy(newUser->usedname, userName);

newUser->chatHistory = (CHAT *)malloc(sizeof(CHAT));
newUser->chatHistory->head = NULL;
newUser->chatHistory->tail = NULL;

userDirectory[index] = newUser;
printf("User created with userid %s at index %d\n", userId, index);
}

USER *findUser(char *userId){
trimNewline(userId);
int index = hashFunction(userId);
int orgIndex = index;

while(userDirectory[index] != NULL){
if(strcmp(userDirectory[index]->useId, userId) == 0){
return userDirectory[index];
}
index = (index + 1) % MAX_USER;

if(index == orgIndex) break;
}
return NULL;
}

void sendMessage(USER *receiver, const char *messageContent, USER *sender){
MESSAGE *newMessage = (MESSAGE *)malloc(sizeof(MESSAGE));
strcpy(newMessage->content, messageContent);
strcpy(newMessage->sender, sender->usedname);
newMessage->next = NULL;
newMessage->prev = receiver->chatHistory->tail;

if(receiver->chatHistory->tail == NULL){
receiver->chatHistory->head = newMessage;
receiver->chatHistory->tail = newMessage;
}
else{
receiver->chatHistory->tail->next = newMessage;
receiver->chatHistory->tail = newMessage;
}
printf("\nMessage sent from %s to %s\n", sender->usedname, receiver->usedname);
}

void listMessages(USER *user){
printf("Chat history for %s:\n", user->usedname);
MESSAGE *message = user->chatHistory->head;

while(message != NULL){
printf("From %s: %s\n", message->sender, message->content);
message = message->next;
}
}

int main(){
for(int i = 0; i < MAX_USER; i++){
userDirectory[i] = NULL;
}
int numUser, numMessage;
char userId[50], userName[50], senderId[50], messageContent[256];
printf("Enter the number of users: ");
scanf("%d", &numUser);

for(int i = 0; i < numUser; i++){
printf("\nEnter User ID: ");
scanf("%s", userId);
printf("Enter User Name: ");
scanf("%s", userName);
createUser(userId, userName);
}

printf("\nEnter the number of messages to send: ");
scanf("%d", &numMessage);
for(int i = 0; i < numMessage; i++){

printf("\nEnter Receiver User ID: ");
scanf("%s", userId);
printf("Enter Sender User ID: ");
scanf("%s", senderId);
printf("Enter the message content: ");
getchar();
fgets(messageContent, sizeof(messageContent), stdin);
messageContent[strcspn(messageContent, "\n")] = 0;

USER *receiver = findUser(userId);
USER *sender = findUser(senderId);

if(receiver == NULL){
printf("\nReceiver Not Found\n");
continue;
}
if(sender == NULL){
printf("\nSender Not Found\n");
continue;
}
sendMessage(receiver, messageContent, sender);

printf("\nEnter User ID to display chat history: ");
scanf("%s", userId);

USER *user = findUser(userId);

if(user != NULL){
listMessages(user);
}
else{
printf("\nUser Not Found\n");
}
}
return 0;
}