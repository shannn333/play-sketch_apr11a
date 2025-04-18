const int ledPin = 3;
const int buzzerPin = 8;
const int buttonPin = 2;

bool roundActive = false;
unsigned long ledOnTime = 0;
const unsigned long reactionWindow = 200; // 0.2 秒
int roundCount = 0;
int successCount = 0;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  Serial.begin(9600);
  Serial.println("按下按鈕開始遊戲！");
}

void loop() {
  if (!roundActive && digitalRead(buttonPin) == LOW) {
    delay(300); // 去彈跳
    startGame();
  }

  if (roundActive) {
    if (digitalRead(buttonPin) == LOW) {
      unsigned long reactionTime = millis() - ledOnTime;
      if (reactionTime <= reactionWindow) {
        successCount++;
        Serial.println("✅ 快速反應成功！");
      } else {
        Serial.println("❌ 太慢了！");
      }
      roundCount++;
      digitalWrite(ledPin, LOW);
      delay(500);
      nextRound();
    } else if (millis() - ledOnTime > reactionWindow) {
      Serial.println("⌛ 超時！");
      roundCount++;
      digitalWrite(ledPin, LOW);
      delay(500);
      nextRound();
    }
  }
}

void startGame() {
  Serial.println("🎮 遊戲開始！");
  roundCount = 0;
  successCount = 0;
  delay(500);
  nextRound();
}

void nextRound() {
  if (roundCount >= 5) {
    endGame();
    return;
  }

  delay(random(1000, 3000)); // 等待時間
  digitalWrite(ledPin, HIGH);
  ledOnTime = millis();
  roundActive = true;
}

void endGame() {
  roundActive = false;
  Serial.print("🎯 成功次數：");
  Serial.println(successCount);

  if (successCount >= 3) {
    Serial.println("🎉 恭喜你成功通過本局！");
    playMarioWinTone();
  } else {
    Serial.println("💀 很遺憾，挑戰失敗！");
    failTone();
  }

  Serial.println("按下按鈕重新開始。");
}

// 🎵 馬力歐勝利音效
void playMarioWinTone() {
  tone(buzzerPin, 660, 100); delay(150);
  tone(buzzerPin, 660, 100); delay(300);
  tone(buzzerPin, 660, 100); delay(300);
  tone(buzzerPin, 510, 100); delay(100);
  tone(buzzerPin, 660, 100); delay(300);
  tone(buzzerPin, 770, 100); delay(550);
  tone(buzzerPin, 380, 100); delay(575);
  noTone(buzzerPin);
}

// 💀 失敗音效
void failTone() {
  for (int i = 0; i < 2; i++) {
    tone(buzzerPin, 300, 200);
    delay(250);
  }
  noTone(buzzerPin);
}
