const int pin1 = 14;
const int pin2 = 15;
const int resetPin = 12;
const int upper1 = 560;
const int lower1 = 460;
const int upper2 = 10000;
const int lower2 = -1;
const int maxNum = 50; // number of nodes to keep in queue
int numNodes = 0;

typedef struct Node {
    int val;
    struct Node *next;
} Node;

void setup() {
    Serial.begin(9600);
    
    pinMode(resetPin, INPUT_PULLUP);
    pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
    Node *head1 = NULL;
    Node *tail1 = NULL;
    Node *head2 = NULL;
    Node *tail2 = NULL;
    int read1;
    int read2;
    
    // While nothing is triggered
    do {
        read1 = analogRead(pin1);
        read2 = analogRead(pin2);
        
        if (numNodes >= maxNum) { // queue is full, remove node
            dequeue(&head1);
            dequeue(&head2);
        }

        // Add elements
        enqueue(&head1, &tail1, read1);
        enqueue(&head2, &tail2, read2);
        numNodes++;
    } while (read1 > lower1 && read1 < upper1 && read2 > lower2 && read2 < upper2);

    // Print queues and restore memory
    while (head1 != NULL && head2 != NULL) {
        Serial.printf("%d, %d\r\n", head1->val, head2->val);
        Node *tmp1 = head1->next;
        Node *tmp2 = head2->next;
        free(head1);
        free(head2);
        head1 = tmp1;
        head2 = tmp2;
    }

    digitalWrite(LED_BUILTIN, HIGH);
    while (digitalRead(resetPin) == HIGH);
    digitalWrite(LED_BUILTIN, LOW);
    Serial.printf("Restarting...\n");
    numNodes = 0;
    delay(1000);
}

void enqueue(Node **head, Node **tail, int num) {
    Node *newNode = (Node *) malloc(sizeof(Node));
    newNode->val = num;
    newNode->next = NULL;

    if (*head == NULL) { // queue is empty so set head and tail
        *head = newNode;
        *tail = newNode;
    } else { // queue is not empty so add node and update tail
        (*tail)->next = newNode;
        *tail = newNode;
    }
}

void dequeue(Node **head) {
    if (*head != NULL) {
        Node *tmp = (*head)->next;
        free(*head);
        *head = tmp;
    }
}
