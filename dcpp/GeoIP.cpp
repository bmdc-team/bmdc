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

const string& GeoIP::getCountry(const string& ip) const {
	Lock l(cs);
	/*if(geo) {
		auto id = (v6() ? GeoIP_id_by_addr_v6 : GeoIP_id_by_addr)(geo, ip.c_str());
		if(id > 0 && static_cast<size_t>(id) < cache.size()) {
			return cache[id];
		}
	}*/
	open();
	int gai_error = 0, mmdb_error = 0;
    MMDB_lookup_result_s result =
        MMDB_lookup_string(&mmdb, ip.c_str(), &gai_error, &mmdb_error);

    if (gai_error != 0 ) {
        dcdebug(
                "\n  Error from getaddrinfo for %s - %s\n\n",
                ip.c_str(), gai_strerror(gai_error));
         return Util::emptyString;       
    }

    if (MMDB_SUCCESS != mmdb_error) {
        dcdebug(
                "\n  Got an error from libmaxminddb: %s\n\n",
                MMDB_strerror(mmdb_error));
		return Util::emptyString;
    }
    
	MMDB_entry_data_s entry_data;
	int status =   MMDB_get_value(&result.entry, &entry_data,
                   "names", "en", NULL);
	if (status != MMDB_SUCCESS) { return Util:emptyString; }
		if (entry_data.has_data) { 
			if(entry_data.type == MMDB_DATA_TYPE_UTF8_STRING)
				return strdup(entry_data.utf8_string);
			
			}

     dcdebug(
            "\n  No entry for this IP address (%s) was found\n\n",
            ip.c_str());
	return Util::emptyString;
}
const string GeoIP::getCountryAB(const string& ip) const {
    Lock l(cs);
    //if(geo)
    //{
     ///   auto id = v6() ? GeoIP_id_by_addr_v6(geo,ip.c_str()) : GeoIP_id_by_addr(geo,ip.c_str());
     //   if(id > 0)
     //   {
     //       return GeoIP_code_by_id(id);
     ///   }
     //}
    /*int gai_error = -1, mmdb_error = -1;
    MMDB_lookup_result_s result = MMDB_lookup_string(&mmdb, ip.c_str(), &gai_error, &mmdb_error);

    if (gai_error != 0) {
        fprintf(stderr,
                "\n  Error from getaddrinfo for %s - %s\n\n",
                ip.c_str(), gai_strerror(gai_error));
         return string("");       
    }

    if (MMDB_SUCCESS != mmdb_error) {
        fprintf(stderr,
                "\n  Got an error from libmaxminddb: %s\n\n",
                MMDB_strerror(mmdb_error));
		return string("");
    }
    
	MMDB_entry_data_s entry_data;
	int status =   MMDB_get_value(&result.entry, &entry_data,
                   "country_iso_code", "en", NULL);
	if (status != MMDB_SUCCESS) { return ""; }
		if (entry_data.has_data) { return strdup(entry_data.utf8_string);}

     fprintf(
            stderr,
            "\n  No entry for this IP address (%s) was found\n\n",
            ip.c_str());
        return "Unknows";    
    */
    return Util::emptyString;
}

void GeoIP::update() {
	Lock l(cs);

	//close();
#ifdef _WIN32
	if(decompress()) {
#endif		
		open();
#ifdef _WIN32		
	}
#endif	
}

namespace {

inline string forwardRet(const char* ret) {
	return ret ? ret : Util::emptyString;
}

#ifdef _WIN32
string getGeoInfo(int id, GEOTYPE type) {
	/*id = GeoIP_Win_GEOID_by_id(id);
	if(id) {
		tstring str(GetGeoInfo(id, type, 0, 0, 0), 0);
		str.resize(GetGeoInfo(id, type, &str[0], str.size(), 0));
		if(!str.empty()) {
			return Text::fromT(str);
		}
	}*/
	return Util::emptyString;
}
#endif

} // unnamed namespace

void GeoIP::rebuild() {
	/*Lock l(cs);
	if(geo) {
		const string& setting = SETTING(COUNTRY_FORMAT);

		auto size = GeoIP_num_countries();
		cache.resize(size);
		for(unsigned int id = 1; id < size; ++id) {

			ParamMap params;

			params["2code"] = forwardRet(GeoIP_code_by_id(id)); 
			params["3code"] = forwardRet(GeoIP_code3_by_id(id)); 
			params["continent"] = forwardRet(GeoIP_continent_by_id(id)); 
			params["engname"] = forwardRet(GeoIP_name_by_id(id)); 
/*#ifdef _WIN32
			params["name"] = [id]() -> string {
				auto str = getGeoInfo(id, GEO_FRIENDLYNAME);
				return str.empty() ? forwardRet(GeoIP_name_by_id(id)) : str;
			};
			params["officialname"] = [id]() -> string {
				auto str = getGeoInfo(id, GEO_OFFICIALNAME);
				return str.empty() ? forwardRet(GeoIP_name_by_id(id)) : str;
			};
#else*/
			/// @todo any way to get localized country names on non-Windows?
//			params["name"] =  forwardRet(GeoIP_name_by_id(id)); 
//			params["officialname"] = forwardRet(GeoIP_name_by_id(id)); 
//#endif

//			cache[id] = Util::formatParams(setting, params);
//		}
//	}
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
	//geo = GeoIP_open(path.c_str(), GEOIP_STANDARD);
	//if(geo) {
	//	GeoIP_set_charset(geo, GEOIP_CHARSET_UTF8);
	//}
	int status = MMDB_open(path.c_str(), MMDB_MODE_MMAP, &mmdb);

    if (MMDB_SUCCESS != status) {
        dcdebug("\n  Can't open %s - %s\n",
                path.c_str(), MMDB_strerror(status));

        if (MMDB_IO_ERROR == status) {
            dcdebug("    IO error: %s\n", strerror(errno));
        }
        
    }
}

void GeoIP::close() {
	//cache.clear();
	MMDB_close(&mmdb);
	//GeoIP_delete(geo);
	//geo = NULL;
}

bool GeoIP::v6() const {
	return true;
	//return geo->databaseType == GEOIP_COUNTRY_EDITION_V6 || geo->databaseType == GEOIP_LARGE_COUNTRY_EDITION_V6;
}

} // namespace dcpp
