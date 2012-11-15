#include "moTimer.h"

#ifdef MO_WIN32
  #include "date_time/posix_time/posix_time.hpp"
#else
  #include "boost/date_time/posix_time/posix_time.hpp"
#endif

using namespace boost::posix_time;

/** ticks desde el arranque de la aplicación*/

MOulong moGetTicksAbsolute() {

    ptime t( microsec_clock::local_time() );

    time_duration clockt = t.time_of_day();

    return clockt.total_milliseconds();
}



#include "moArray.h"
moDefineDynamicArray( moTimers )

static moTimerAbsolute*     GlobalMoldeoTimer = new moTimerAbsolute();

moTimerAbsolute::moTimerAbsolute() {
    on = false;
    pause_on = false;
    start_tick = 0;
    start_last = 0;
    duration = 0;
}

moTimerAbsolute::moTimerAbsolute( const moTimerAbsolute &src) {
    (*this) = src;
}

moTimerAbsolute::~moTimerAbsolute() {
}

moTimerAbsolute&
moTimerAbsolute::operator = (const moTimerAbsolute &src) {
    on = src.on;
	pause_on = src.pause_on;
	start_tick = src.start_tick;
	start_last = src.start_last;
	duration = src.duration;
	last_duration = src.last_duration;
	return (*this);
}


void moTimerAbsolute::Start() {
    on = true;
    pause_on = false;
    start_tick = moGetTicksAbsolute();
}

long moTimerAbsolute::Duration() {
    if (on) {
        start_last = moGetTicksAbsolute();
        last_duration = duration;
        if (!pause_on) duration = start_last - start_tick;
        else start_tick = start_last - last_duration;
    } else duration = 0;
    return duration;
}

void moTimerAbsolute::SetDuration( MOulong p_timecode ) {
    start_tick = moGetTicksAbsolute()-p_timecode;
    start_last = moGetTicksAbsolute();
    duration = p_timecode;
    last_duration = duration;
}


moTimerState
moTimerAbsolute::State() const {

    if ( Paused() ) {
        return MO_TIMERSTATE_PAUSED;
    } else if ( Started() ) {
        return MO_TIMERSTATE_PLAYING;
    }
    return MO_TIMERSTATE_STOPPED;
}


void  moStartTimer() {
    GlobalMoldeoTimer->Start();
}

void  moPauseTimer() {
    GlobalMoldeoTimer->Pause();
}

void  moContinueTimer() {
   GlobalMoldeoTimer->Continue();
}

void  moStopTimer() {
    GlobalMoldeoTimer->Stop();
}

void moSetDuration( MOulong p_timecode ) {
    long test = 0;
    test = p_timecode;
    GlobalMoldeoTimer->SetDuration( test );
}

MOulong moGetDuration() {
    return GlobalMoldeoTimer->Duration();
}

MOulong moGetTicks() {
    return GlobalMoldeoTimer->Duration();
}


moTimerState moGetTimerState() {
    return GlobalMoldeoTimer->State();
}


bool moIsTimerPaused() {
    return GlobalMoldeoTimer->State()==MO_TIMERSTATE_PAUSED;
}


bool moIsTimerStopped() {
    return GlobalMoldeoTimer->State()==MO_TIMERSTATE_STOPPED;
}

bool moIsTimerStarted() {
    return !moIsTimerStopped();
}

bool moIsTimerPlaying() {
    return GlobalMoldeoTimer->State()==MO_TIMERSTATE_PLAYING;
}


//==================
//  CLASS moTimer
//===================

moTimer::moTimer() : moTimerAbsolute() {
    m_TimerId = -1;
    m_ObjectId = -1;
    m_pRelativeTimer = NULL;
}

moTimer::moTimer( moTimer& src ) : moTimerAbsolute() {
    *this = src;
}

moTimer::~moTimer() {
}

moTimer&
moTimer::operator=(moTimer& src) {
    (moTimerAbsolute&)(*this)=(moTimerAbsolute&)src;
    m_TimerId = src.m_TimerId;
    m_ObjectId = src.m_ObjectId;
    m_pRelativeTimer = src.m_pRelativeTimer;
    return *this;
}

void moTimer::Start() { ///Also Reset to 0
    on = true;
    pause_on = false;
    duration = 0;
    last_duration = duration;

    if (m_pRelativeTimer) ///relative to another timer
    {
        start_tick = m_pRelativeTimer->Duration();
    } else {
        start_tick = moGetTicks();
    }

}

long moTimer::Duration() {

    if (on) {
        //esta vez devuelve el Duration del GlobalMoldeoTimer....

        if (m_pRelativeTimer) ///relative to another timer
        {
            if (m_pRelativeTimer->Started()) {
                start_last = m_pRelativeTimer->Duration();
            } else {
                start_last = 0;
                start_tick = 0;
                duration = 0;
                last_duration = duration;
            }

        }
        else ///Depends on global Moldeo Timer
        {
                if (moIsTimerStarted()) {
                    start_last = moGetTicks();
                } else {
                    start_last = 0;
                    start_tick = 0;
                    duration = 0;
                    last_duration = duration;
                }
        }

        last_duration = duration;
        if (!pause_on) duration = start_last - start_tick;
        else start_tick = start_last - last_duration;
    } else duration = 0;

    return duration;
}

void moTimer::SetDuration( MOulong p_timecode ) {
    if (m_pRelativeTimer) {
        start_tick = m_pRelativeTimer->Duration()-p_timecode;
        start_last = m_pRelativeTimer->Duration();
    } else {
        start_tick = moGetTicks()-p_timecode;
        start_last = moGetTicks();
    }

    duration = p_timecode;
    last_duration = duration;
}

void moTimer::Fix() {
    if (m_pRelativeTimer) {
        start_tick = m_pRelativeTimer->Duration();
    } else {
        start_tick = moGetTicks();
    }

}

