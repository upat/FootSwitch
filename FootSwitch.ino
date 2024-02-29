#include "DigiKeyboard.h"

#define ON  1
#define OFF 0

class SwSampling
{
  private:
    uint8_t _sw_port;      /* 読み出しポート番号 */
    uint8_t _sw_input;     /* ポート読み出し値 */
    uint8_t _sw_on_count;  /* ON判定電圧読み出し回数 */
    uint8_t _sw_off_count; /* OFF判定電圧読み出し回数 */
    uint8_t _sw_on;        /* ON判定電圧(LOW/HIGH) */
    uint8_t _sw_off;       /* OFF判定電圧(HIGH/LOW) */
    uint16_t _sw_timer_count; /* 連打防止タイマカウント値 */

    const uint8_t _sw_filter = 5;   /* フィルタ回数 */
    const uint16_t _sw_timer = 0; /* 連打防止タイマ設定値(4ms*250=1s) */

    void TimerCount( void ); /* 連打防止タイマカウント処理 */
    void TimerSet( void );   /* 連打防止タイマ設定処理 */
  public:
    SwSampling( uint8_t port, uint8_t mode, uint8_t active ); /* コンストラクタ */
    void Sampling( void ); /* サンプリング処理 */
    uint8_t sw_state;      /* SW状態(確定値) */
    bool oneshot_flag;     /* ONになった初回のみキー送信を行うフラグ */
};

SwSampling::SwSampling( uint8_t port, uint8_t mode, uint8_t active )
{
  pinMode( port, mode );

  _sw_port = port;
  _sw_input = 0;
  _sw_on_count = 0;
  _sw_off_count = 0;
  _sw_timer_count = 0;

  if( HIGH == active )
  {
    /* HIGHがONの時 */
    _sw_on = HIGH;
    _sw_off = LOW;
  }
  else
  {
    /* LOWがONの時 */
    _sw_on = LOW;
    _sw_off = HIGH;
  }

  sw_state = OFF;
  oneshot_flag = true;
}

void SwSampling::Sampling( void )
{
  /* ポート読み出し */
  _sw_input = digitalRead( _sw_port );
  TimerCount();

  /* ON/OFF判定 */
  if( _sw_on == _sw_input )
  {
    _sw_on_count++;
    _sw_off_count = 0;
  }
  else if( _sw_off == _sw_input )
  {
    _sw_on_count = 0;
    _sw_off_count++;
  }
  else
  {
    /* 通常入らない経路 */
    _sw_on_count = 0;
    _sw_off_count = 0;
  }

  /* _sw_filter以上でON/OFF確定(OFF優先) */
  if( _sw_filter <= _sw_off_count )
  {
    sw_state = OFF;
    /* _sw_on_count = 0; */ /* 冗長 */
    _sw_off_count = _sw_filter; /* オーバーフロー防止 */
  }
  else if( _sw_filter <= _sw_on_count )
  {
    /* タイマが0(最後のON確定から1s以上経過) */
    if( 0 == _sw_timer_count )
    {
      sw_state = ON;
      _sw_on_count = _sw_filter; /* オーバーフロー防止 */
      /* _sw_off_count = 0; */ /* 冗長 */

      TimerSet();
    }
  }
  else
  {
    /* 何もしない */
  }
}

void SwSampling::TimerCount( void )
{
  if( 0 < _sw_timer_count )
  {
    _sw_timer_count--;
  }
}

void SwSampling::TimerSet( void )
{
  _sw_timer_count = _sw_timer;
}

/* PB0設定(LOWがON) GNDと結線 */
SwSampling sw0 = SwSampling( 0, INPUT_PULLUP, LOW );
/* PB1設定(HIGHがON) VCC(5V)と結線 */
//SwSampling sw0 = SwSampling( 1, INPUT, HIGH );
/* PB2設定(LOWがON) GNDと結線 */
//SwSampling sw0 = SwSampling( 2, INPUT_PULLUP, LOW );

void setup()
{
  /* 処理なし */
}

void loop()
{ 
  /* サンプリング処理 */
  sw0.Sampling();

  if( ON == sw0.sw_state )
  {
    /* 連打を避けるため、SW押しっぱなしでも一度だけ送信 */
    if( sw0.oneshot_flag )
    {
      /* sendKeyStroke:押して離す、sendKeyPress:押したまま(sendKeyPress( 0 )で離す) */
      //DigiKeyboard.sendKeyStroke( 0 ); /* All Release */
      //DigiKeyboard.sendKeyStroke( KEY_M, MOD_CONTROL_LEFT | MOD_SHIFT_LEFT ); /* Ctrl+Shift+M */
      DigiKeyboard.sendKeyPress( 0, MOD_CONTROL_RIGHT ); /* Right Control */
      sw0.oneshot_flag = false;
    }
  }
  else
  {
    DigiKeyboard.sendKeyPress( 0 ); /* All Release */
    sw0.oneshot_flag = true;
  }

  /* 4ms待ち */
  DigiKeyboard.delay( 4 );
}
