#ifndef _TEMPLET_DATA_
#define _TEMPLET_DATA_

#include <string>
#include <cstdlib>
using namespace std;

class Templet_Data
{
public:
        Templet_Data() {
                _btype = _etype = _ftype = -1;
                _dead = false;
                _next = NULL;
                _sunext = NULL;
        }
        ~Templet_Data() {
                _next = NULL;
        }
        string getBeg() {
                return _beg;
        }
        string getEnd() {
                return _end;
        }
        string getFixed() {
                return _fixed;
        }
        bool getDead() {
                return _dead;
        }
        void setDead(bool value) {
                _dead = value;
        }
        void setNext(Templet_Data* value = NULL) {
                _next = value;
        }
        void setSunext(Templet_Data* value = NULL) {
                _sunext = value;
        }
        Templet_Data* getNext() {
                if (_next != NULL)
                        return _next;
                else
                        return NULL;
        }
        Templet_Data* getSunext() {
                if (_sunext != NULL)
                        return _sunext;
                else
                        return NULL;
        }
        int splitBuf(string value) {
                if (value.empty())
                        return 0;
                int tmptype = atoi(value.c_str());
                if (tmptype > 9 || tmptype < 0)
                        return 0;
                if ( 4 == tmptype || 5 == tmptype) {
                        tmptype -= 4;
                }
                int tmpfind = (signed int)value.find('=');
                if (tmpfind < 1)
                        return 0;
                char tmpchar = value[tmpfind-1];
                if ( 'b' != tmpchar && 'e' != tmpchar && 'f' != tmpchar)
                        return 0;
                string tmpstr;
                tmpstr = value.substr(tmpfind+1,value.size()-tmpfind-1);
                if ( 'b' == tmpchar) {
                        _strid = value.substr(1,tmpfind-3);
                        _beg = tmpstr;
                        _btype = tmptype;
                } else if ( 'e' == tmpchar) {
                        _end = tmpstr;
                        _etype = tmptype;
                } else if ( 'f' == tmpchar) {
                        setFixed(tmpstr);
                        _ftype = tmptype;
                }
                return 1;
        }
        int compareID(string value) {
                if (value.size() <= 0)
                        return 0;
                if (_strid.size() <= 0)
                        return 0;
                if ( (signed int)_strid.find(value) >= 0)
                        return 1;
                else
                        return 0;
        }
        string getBuf(string value, int& tmpflag) {
                string tmpstr = value;
                tmpflag = -1;
                if ( value.empty() )
                        return tmpstr;
                int tmpbeg = (signed int)value.find(_beg,0);
                if ( tmpbeg < 0)
                        return tmpstr;
                if (_btype %2  == 0)
                        tmpbeg += _beg.size();
                int tmpend = (signed  int)value.find(_end,tmpbeg) - 1;
                if (tmpend < 0)
                        return tmpstr;
                if (_etype%2 == 1)
                        tmpend += _end.size();
                tmpstr = value.substr(tmpbeg,tmpend-tmpbeg+1);
                tmpflag = tmpend + 1;
                return tmpstr;
        }

        void setBeg(string& value) {
                if (value.empty()) {
                        _beg = " ";
                } else {
                        _beg = value;
                }
        }
        void setEnd(string& value) {
                if (value.empty()) {
                        _end = " ";
                } else {
                        _end = value;
                }
        }
        void setFixed(string& value) {
                if (value.empty()) {
                        _fixed = " ";
                } else {
                        _fixed = value;
                }
	}
private:
        int _btype;
        int _etype;
        int _ftype;
        string _beg;
        string _end;
        string _fixed;
        bool _dead;
        Templet_Data *_next;
        Templet_Data *_sunext;

public:
        string _strid;
};

#endif

