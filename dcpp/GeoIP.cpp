/*
 * Copyright (C) 2001-2018 Jacek Sieka, arnetheduck on gmail point com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include "stdinc.h"
#include "GeoIPc.h"

#include "File.h"
#include "format.h"
#include "SettingsManager.h"
#include "Util.h"
#include "ZUtils.h"

#include <GeoIP.h>

namespace dcpp {
using namespace std;

static char *
get_value (MMDB_lookup_result_s res, ...)
{
  MMDB_entry_data_s entry_data;
  char *value = NULL;
  int status = 0;
  va_list keys;
  va_start (keys, res);

  status = MMDB_vget_value (&res.entry, &entry_data, keys);
  va_end (keys);

  if (status != MMDB_SUCCESS)
    return NULL;

  if (!entry_data.has_data)
    return NULL;

  if (entry_data.type != MMDB_DATA_TYPE_UTF8_STRING)
    dcdebug ("Invalid data UTF8 GeoIP2 data %d:\n", entry_data.type);

  if ((value = strndup (entry_data.utf8_string, entry_data.data_size)) == NULL)
    dcdebug ("Unable to allocate buffer %s: ", strerror (errno));

  return value;
}


GeoIP::GeoIP(string&& _path) : path(move(_path)) {
#ifdef _WIN32	
	if(File::getSize(path) > 0 
	|| decompress()
	)
#else 
	if(Util::fileExists(path) == true)
#endif	
	{
		open();
	}
}

GeoIP::~GeoIP() {
	Lock l(cs);
	close();
}

const string GeoIP::getCountry(const string& ip) const {
	Lock l(cs);

	int gai_error, mmdb_error;
    	MMDB_lookup_result_s result =
        MMDB_lookup_string(const_cast<MMDB_s*>(&mmdb), ip.c_str(), &gai_error, &mmdb_error);

	if (0 != gai_error){
        dcdebug(
                "\n  Error from getaddrinfo for %s - %s\n\n",
                ip.c_str(), gai_strerror(gai_error));
    	}

    if (mmdb_error != MMDB_SUCCESS ) {
        dcdebug(
                "\n  Got an error from libmaxminddb: %s\n\n",
                MMDB_strerror(mmdb_error));
	}
	char *country = NULL, *code = NULL, *cont = NULL;
	country = get_value (result, "country", "names", "en", NULL);
	code = get_value (result, "country", "iso_code", NULL);
	//cont = get_value (result,"continent", "code" ,NULL);
	
	const string& setting = SETTING(COUNTRY_FORMAT);
	ParamMap params;
	params["2code"] = code;
	//		params["3code"] = forwardRet(GeoIP_code3_by_id(id)); 
	//params["continent"] = cont; 
	params["engname"] = country; 
	params["name"] =  country; 
	params["officialname"] = country; 
	Util::formatParams(setting, params);
	
	return  std::string(""+string(code)+" - "+string(country)+"");
}

const string GeoIP::getCountryAB(const string& ip) const {
    Lock l(cs);
    int gai_error, mmdb_error;
    MMDB_lookup_result_s result = MMDB_lookup_string(const_cast<MMDB_s*>(&mmdb), ip.c_str(), &gai_error, &mmdb_error);

    if (gai_error != 0) {
        fprintf(stderr,
                "\n  Error from getaddrinfo for %s - %s\n\n",
                ip.c_str(), gai_strerror(gai_error));
    }

    if (mmdb_error != MMDB_SUCCESS ) {
        fprintf(stderr,
                "\n  Got an error from libmaxminddb: %s\n\n",
                MMDB_strerror(mmdb_error));
    }
    char *code = NULL;
	code = get_value (result, "country", "iso_code", NULL); 
	return  std::string(code);
}

void GeoIP::update() {
	Lock l(cs);

	//close();
#ifdef _WIN32
//	if(decompress()) {
#endif		
//		open();
#ifdef _WIN32		
	}
#endif	
}

#ifdef _WIN32
bool GeoIP::decompress() const {
	if(File::getSize(path + ".gz") <= 0) {
		return false;
	}

	try { GZ::decompress(path + ".gz", path); }
	catch(const Exception&) { return false; }

	return true;
}
#endif
void GeoIP::open() {
	dcdebug("%s",path.c_str());
	int status = MMDB_open(path.c_str(), MMDB_MODE_MMAP, &mmdb);

    if (status != MMDB_SUCCESS) {
        dcdebug("\n  Can't open %s - %s\n",
                path.c_str(), MMDB_strerror(status));

        if( status == MMDB_IO_ERROR) {
            dcdebug("   IO error: %s\n", strerror(errno));
        }
        
    }
}

void GeoIP::close() {
	MMDB_close(&mmdb);
}

} // namespace dcpp
