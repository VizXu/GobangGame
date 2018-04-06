#include "chessboardpattern.h"

ChessboardPattern::ChessboardPattern(const char* cName = "computer",const char* hName = "human"):
computerName(cName),humanName(hName)
{
    computer = new Computer(this->computerName);
    human    = new Human(this->humanName);
    this->gameStep = 0;
    this->gamelevel = 1;
}

ChessboardPattern::~ChessboardPattern()
{
    delete computer;
    delete human;
}

ChessboardPattern* ChessboardPattern::getPattern()
{
    const char* c = "computer";
    const char* h = "human";
    if(NULL == pattern){
	pattern = new ChessboardPattern(c,h);
	return pattern;
    }
    else{
	return pattern;
    }
}

const char* ChessboardPattern::getComputerName() const
{
    return this->computerName;
}

const char* ChessboardPattern::getHumanName() const 
{
    return this->humanName;
}

void ChessboardPattern::initGame(int size,int level) 
{
    this->gameStep = 0;
    this->setLevel(level);
    this->chessboard.init(size);
}

int ChessboardPattern::getGameStep() const
{
    return this->gameStep;
}

void ChessboardPattern::setLevel(int level)
{
   if(level > 3 || level <= 0) throw std::invalid_argument(std::string("level should be 0-3"));
   this->gamelevel = level;
}

int ChessboardPattern::getLevel() const
{
   return this->gamelevel;
}

void ChessboardPattern::setCurrentGameStatus(struct game_status& status)
{
   this->currentGameStatus = status;
}

struct game_status ChessboardPattern::getCurrentGameStatus() const
{
  return this->currentGameStatus;
}

struct game_status ChessboardPattern::gameRunning(const struct client_info& info)
{
   switch(this->getLevel()){
      case 1: return this->gameRunningLevel1(info);
      case 2: return this->gameRunningLevel1(info);
      case 3: return this->gameRunningLevel1(info);
      default: return this->gameRunningLevel1(info);
   } 
}

struct game_status&& ChessboardPattern::gameRunningLevel1(const struct client_info& info)
{
    int xpos = info.info.xpos;
    int ypos = info.info.ypos;
    struct game_status gameStatus;
    board_position tmp_position;

    this->human->change_state();//start   
    if(!this->human->make_a_step_via_net(chessboard,xpos,ypos)){
	gameStatus.status = INVALID_POS;
	return std::move(gameStatus);
    }
    tmp_position = this->human->get_present_position();
    if(this->test_balance(chessboard)){
	 gameStatus.status = BALANCE;
         return std::move(gameStatus);
    }
    if(this->test_winner(chessboard,tmp_position,'&')){
	 gameStatus.status = CLIENT_WIN;
         return std::move(gameStatus);
    }
    this->human->change_state(); //stop

    this->computer->change_state();//start
    this->computer->make_a_step(chessboard);

    tmp_position = this->computer->get_present_position();
    if(this->test_winner(chessboard,tmp_position,'@')){
	 gameStatus.status = COMPUTER_WIN;
         return std::move(gameStatus);
    }
    this->computer->change_state(); //stop
    this->gameStep += 1;

    this->setCurrentGameStatus(gameStatus);

return std::move(gameStatus);
}

struct game_status&& ChessboardPattern::gameRunningLevel2(const struct client_info& info)
{
    struct game_status gameStatus;
    this->setCurrentGameStatus(gameStatus);
return std::move(gameStatus);
}

struct game_status&& ChessboardPattern::gameRunningLevel3(const struct client_info& info)
{
    struct game_status gameStatus;
    this->setCurrentGameStatus(gameStatus);
return std::move(gameStatus);
}

const Chessboard& ChessboardPattern::getChessboard() const
{
	return this->chessboard;
}

ChessboardPattern* ChessboardPattern::pattern = NULL;
