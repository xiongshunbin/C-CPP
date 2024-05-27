// tv.cpp -- methods for the Tv class (Remote methods are inline)
#include "tv.h"
#include <iostream>

bool Tv::volup()
{
    if(volume < MaxVal)
    {
        volume ++;
        return true;
    }
    else
        return false;
}

bool Tv::voldown()
{
    if(volume > MinVal)
    {
        volume --;
        return true;
    }
    else
        return false;
}

void Tv::chanup()
{
    if(channel < maxchannel)
        channel ++;
    else
        channel = 1;
}

void Tv::chandown()
{
    if(channel > 1)
        channel --;
    else
        channel = maxchannel;
}

void Tv::settings() const
{
    using std::cout;
    using std::endl;
    cout << "TV is " << (state == Off ? "Off" : "On") << endl;
    if(state == On)
    {
        cout << "Volume setting = " << volume << endl;
        cout << "Channel setting = " << channel << endl;
        cout << "Mode = "
             << (mode == Antenna ? "antenna" : "cable") << endl;
        cout << "Input = "
             << (input == TV ? "TV" : "DVD") << endl;
    }
}

void Tv::set_remote_mode(Remote &r)
{
    if(state == On)
        r.set_remote_mode();
}

void Remote::show_remote_mode() const
{
    using std::cout;
    using std::endl;

    cout << "The remote's mode is " 
         << (remote_mode ? "InterActive" : "Normal") << endl;
}

void Remote::set_remote_mode()
{
    remote_mode = (remote_mode == Normal) ? InterActive : Normal;
}
