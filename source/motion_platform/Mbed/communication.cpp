/******************************************************

****┏┓          ┏┓
**┏┛┻━━━━━━┛┻┓
**┃                 ┃
**┃      ━━━      ┃
**┃  ┳┛       ┗┳ ┃
**┃                 ┃
**┃ '''    ┻   ''' ┃
**┃                 ┃
**┗━━┓       ┏━━┛
*******┃       ┃
*******┃       ┃
*******┃       ┃
*******┃       ┗━━━━━━━━┓
*******┃                      ┃━┓
*******┃      NO BUG          ┏━┛
*******┃                      ┃
*******┗━┓  ┓  ┏━┏━┓  ━┛
***********┃  ┛  ┛    ┃  ┛  ┛
***********┃  ┃  ┃    ┃  ┃  ┃
***********┗━┛━┛     ┗━┛━┛

This part is added by project ESDC2014 of CUHK team.
All the code with this header are under GPL open source license.
This program is running on Mbed Platform 'mbed LPC1768' avaliable in 'http://mbed.org'.
**********************************************************/
#include <communication.h>

Communication::Communication(MySerial* _DEBUG, MySerial *_IntelToMbed, MySerial *_MbedToArduino)
{
    this->_DEBUG = _DEBUG;
    this->_IntelToMbed = _IntelToMbed;
    this->_MbedToArduino = _MbedToArduino;
    
    init();
}

Communication::~Communication()
{
    delete[] buffer_IntelToMbed;
    delete[] buffer_MbedToArduino;
    delete _DEBUG;
    delete _IntelToMbed;
    delete _MbedToArduino;
}

void Communication::init()
{
    buffer_IntelToMbed = new uint8_t[BUFFER_SIZE];
    buffer_MbedToArduino = new uint8_t[BUFFER_SIZE];
    in_IntelToMbed = 0;
    out_IntelToMbed = 0;
    in_MbedToArduino = 0;
    out_MbedToArduino = 0;
    state_IntelToMbed = 0;
    state_MbedToArduino = 0;
    check_sum = 0;
    info_ok_IntelToMbed = 0;
    info_ok_MbedToArduino = 0;
}

uint8_t Communication::getByte(uint8_t communication_type)
{
    uint8_t _x = 0;
    if(communication_type == 0)
    {
        _x = buffer_IntelToMbed[out_IntelToMbed++];
        if(out_IntelToMbed == BUFFER_SIZE)
        {
            out_IntelToMbed &= 0x00;
        }
    }
    else if(communication_type == 1)
    {
        _x = buffer_MbedToArduino[out_MbedToArduino++];
        if(out_MbedToArduino == BUFFER_SIZE)
        {
            out_MbedToArduino &= 0x00;
        }
    }
    return _x;
}

uint16_t Communication::get2Bytes(uint8_t communication_type)
{
    uint8_t byte1 = getByte(communication_type);
    uint8_t byte2 = getByte(communication_type);
    return uint16_t((byte1 << 8) | byte2);
}

void Communication::putByte(uint8_t _x, uint8_t _i)
{
    //Serial.write(_x);//For Arduino
     //For Mbed
    if(_i == 0) //_DEBUG
    {
        _DEBUG->putc(_x);
    }
    else if(_i == 1) //IntelToMbed
    {
        _IntelToMbed->putc(_x);
    }
    else if(_i == 2) //MbedToArduino
    {
        _MbedToArduino->putc(_x);
    }
}

void Communication::put2Bytes(uint16_t _x, uint8_t _i)
{
    putByte(uint8_t(_x >> 8), _i);
    putByte(uint8_t(_x & 0x0f), _i);
}

void Communication::putToBuffer(uint8_t _x, uint8_t communication_type)
{
    if(communication_type == 0)
    {
        buffer_IntelToMbed[in_IntelToMbed++] = _x;
        if(in_IntelToMbed == BUFFER_SIZE)
        {
            in_IntelToMbed &= 0x00;
        }
    }
    else if(communication_type == 1)
    {
        buffer_MbedToArduino[in_MbedToArduino++] = _x;
        if(in_MbedToArduino == BUFFER_SIZE)
        {
            in_MbedToArduino &= 0x00;
        }
    }
}

void Communication::parseMessage()
{
    if(in_IntelToMbed != out_IntelToMbed)
    {
        uint8_t _x = getByte(0);
        switch(state_IntelToMbed)
        {
            case 0: //checking starter
            {
                if(DEBUG_ON)
                {
                    putByte('0', 1);
                }
                check_sum = 0;

                if(_x == STARTER)
                {
                    state_IntelToMbed++;
                }
                else
                {
                    state_IntelToMbed = 0;
                }
                break;
            }

            case 1: //checking action_type
            {
                if(DEBUG_ON)
                {
                    putByte('1', 1);
                }
                check_sum += _x;
                action_type = _x;
                if(action_type == 0 || action_type == 1 || action_type == 2)
                {
                    state_IntelToMbed++;
                }
                else
                {
                    state_IntelToMbed = 0;
                }
                break;
            }

            case 2: //move_dis upper 4 bits
            {
                if(DEBUG_ON)
                {
                    putByte('2', 1);
                }
                check_sum += _x;
                move_dis = _x << 8;
                state_IntelToMbed++;
                break;
            }

            case 3: //move_dis lower 4 bits
            {
                if(DEBUG_ON)
                {
                    putByte('3', 1);
                }
                check_sum += _x;
                move_dis |=  _x;
                state_IntelToMbed++;
                break;
            }

            case 4: //move_dir
            {
                if(DEBUG_ON)
                {
                    putByte('4', 1);
                }
                check_sum += _x;
                move_dir = _x;
                if((action_type == 0 && (move_dir == 0 || move_dir == 1 || move_dir == 2 || move_dir == 3)) || (action_type == 1 && (move_dir == 0 || move_dir == 2)) || action_type == 2)
                {
                    state_IntelToMbed++;
                }
                else
                {
                    state_IntelToMbed = 0;
                }
                break;
            }

            case 5: //rotate_dis upper 4 bits
            {
                if(DEBUG_ON)
                {
                    putByte('5', 1);
                }
                check_sum += _x;
                rotate_dis = _x << 8;
                state_IntelToMbed++;
                break;
            }

            case 6: //rotate_dis lower 4 bits
            {
                if(DEBUG_ON)
                {
                    putByte('6', 1);
                }
                check_sum += _x;
                rotate_dis |= _x;
                state_IntelToMbed++;
                break;
            }

            case 7: //rotate_dir
            {
                if(DEBUG_ON)
                {
                    putByte('7', 1);
                }
                check_sum += _x;
                rotate_dir = _x;
                if((action_type == 1 && ((rotate_dir >> 6) == 0)) || ((action_type == 0 || action_type == 2) && ((rotate_dir >> 6) == 3)))
                {
                    state_IntelToMbed++;
                }
                else
                {
                    state_IntelToMbed = 0;
                }
                break;
            }

            case 8: //check_sum
            {
                if(DEBUG_ON)
                {
                    putByte('8', 1);
                }
                if(check_sum == _x)
                {
                    switch(action_type)
                    {
                        case 0: //car movement
                        info_ok_IntelToMbed = 1;
                        break;

                        case 1: //lifter
                        info_ok_IntelToMbed = 2;
                        break;

                        case 2: //camera platform
                        info_ok_IntelToMbed = 3;
                        break;

                        default:
                        info_ok_IntelToMbed = 0; //not ok
                        break;
                    }
                }
                state_IntelToMbed  = 0;
                break;
            }

            default:
            {
                state_IntelToMbed = 0;
                break;
            }
        }
    }
}

void Communication::forwardMessage()
{
    //message structure is defined in source/motion_platform/intel_board/lib/message.h
    uint8_t i = out_IntelToMbed - 9; //message size is 9 bytes
    putByte(buffer_IntelToMbed[i++], 2); //starter, 2 means MbedToArduino
    putByte(buffer_IntelToMbed[i++], 2); //action_type
    putByte(buffer_IntelToMbed[i++], 2); //move_dis
    putByte(buffer_IntelToMbed[i++], 2);
    putByte(buffer_IntelToMbed[i++], 2); //move_dir
    putByte(buffer_IntelToMbed[i++], 2); //rotate_dis
    putByte(buffer_IntelToMbed[i++], 2);
    putByte(buffer_IntelToMbed[i++], 2); //rotate_dir
    putByte(buffer_IntelToMbed[i++], 2); //checksum
}

void Communication::ACK(Lifter* lifter, Camera_platform* camera_platform)
{
    if(action_type == 0) //car movement
    {
        while(info_ok_MbedToArduino != 1)
        {
            if(in_MbedToArduino != out_MbedToArduino)
            {
                uint8_t _x = getByte(1);
                switch(state_MbedToArduino)
                {
                    case 0: //checking starter
                    {
                        //putByte('0', 1);
                        if(_x == STARTER)
                        {
                            state_MbedToArduino++;
                        }
                        else
                        {
                            state_MbedToArduino = 0;
                        }
                        break;
                    }

                    case 1: //checking 'O'
                    {
                        //putByte('1', 1);
                        if(_x == 0x4f) //O
                        {
                            state_MbedToArduino++;
                        }
                        else
                        {
                            state_MbedToArduino = 0;
                        }
                        break;
                    }

                    case 2: //checking 'K'
                    {
                        //putByte('2', 1);
                        if(_x == 0x4b) //K
                        {
                            state_MbedToArduino++;
                        }
                        else
                        {
                            state_MbedToArduino = 0;
                        }
                        break;
                    }

                    case 3: //checking check_sum_MbedToArduino
                    {
                        //putByte('3', 1);
                        if(_x == 0x9a) //checksum
                        {
                            info_ok_MbedToArduino = 1;
                        }
                        
                        state_MbedToArduino = 0;
                        break;
                    }

                    default:
                    {
                        state_MbedToArduino = 0;
                        info_ok_MbedToArduino = 0;
                        break;
                    }
                }
            }
        }
    }
    else if(action_type == 1) //lifter
    {
        uint32_t pulseCountOld = 0;
        uint32_t pulseCountNew = 0;
        while(!lifter->isStopped())
        {
            pulseCountOld = lifter->pulseCount;
            wait_ms(50);
            pulseCountNew = lifter->pulseCount;
            if(pulseCountOld == pulseCountNew)
            {
                break;
            }
        }
    }
    else if(action_type == 3)
    {

    putByte(STARTER ,1); //1 means IntelToMbed
    putByte(0x4f ,1); //O
    putByte(0x4b ,1); //K
    putByte(0x9a ,1); //check_sum = 0xaf + 0x4b = 0x9a
    
    return;
    } 
    putByte(STARTER ,1); //1 means IntelToMbed
    putByte(0x4f ,1); //O
    putByte(0x4b ,1); //K
    putByte(0x9a ,1); //check_sum = 0xaf + 0x4b = 0x9a
}

uint8_t Communication::getInfoOK(uint8_t communication_type)
{
    if(communication_type == 0)
    {
        return info_ok_IntelToMbed;
    }
    else if(communication_type == 1)
    {
        return info_ok_MbedToArduino;
    }
    return 0; //error
}

void Communication::resetInfoOK(uint8_t communication_type)
{
    if(communication_type == 0)
    {
        info_ok_IntelToMbed = 0;
    }
    else if(communication_type == 1)
    {
        info_ok_MbedToArduino = 0;
    }
}

uint16_t Communication::getMoveDis()
{
    return move_dis;
}

uint16_t Communication::getRotateDis()
{
    return rotate_dis;
}

uint8_t Communication::getMoveDir()
{
    return move_dir;
}

uint8_t Communication::getRotateDir()
{
    return rotate_dir;
}