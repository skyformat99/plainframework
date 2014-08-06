#include "pf/base/time_manager.h"
#include "pf/base/log.h"
#include "common/define/enum.h"
#include "connection/login.h"

namespace connection {

Login::Login() {
  __ENTER_FUNCTION
    clear();
  __LEAVE_FUNCTION
}

Login::~Login() {
  //do nothing
}

bool Login::init() {
  __ENTER_FUNCTION
    resetkick();
    if (!Base::init()) return false;
    return true;
  __LEAVE_FUNCTION
    return false;
}

void Login::resetkick() {
  __ENTER_FUNCTION
    kicktime_ = TIME_MANAGER_POINTER->get_current_time();
  __LEAVE_FUNCTION
}

void Login::clear() {
  __ENTER_FUNCTION
    memset(account_, 0, sizeof(account_));
  __LEAVE_FUNCTION
}

bool Login::isplayer() const {
  return true;
}

bool Login::isserver() const {
  return false;
}

bool Login::heartbeat(uint32_t time) {
  __ENTER_FUNCTION
    if (time > kicktime_ + CONNECTION_KICKTIME_MAX) {
      SLOW_ERRORLOG(APPLICATION_NAME,
                    "[connection] (Login::heartbeat)"
                    " didn't receive message for long time. Kicked!");
      return false;
    }
    if (getstatus() == kPlayerStatusLoginProcessTurn) {
      if (time > last_sendmessage_turntime_ + CONNECTION_TURNMESSAGE_TIME_MAX) {
        last_sendmessage_turntime_ = time;
        uint32_t turnnumber = 0;
      }
    }
    if (!Base::heartbeat(time)) return false;
    return true;
  __LEAVE_FUNCTION
    return false;
}

bool Login::flushdirectly(pf_net::packet::Base *packet) {
  __ENTER_FUNCTION
    bool result = Base::sendpacket(packet);
    if (!result || !Base::processoutput()) return false;
    return true;
  __LEAVE_FUNCTION
    return false;
}

bool Login::senddirectly(pf_net::packet::Base *packet) {
  __ENTER_FUNCTION
    bool result = Base::sendpacket(packet);
    if (result) {
      int8_t count = 0;
      result = Base::processoutput();
      ++count;
    }
    return result;
  __LEAVE_FUNCTION
    return false;
}

} //namespace connection
