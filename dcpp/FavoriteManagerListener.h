/*
 * Copyright (C) 2001-2017 Jacek Sieka, arnetheduck on gmail point com
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

#ifndef DCPLUSPLUS_DCPP_FAVORITEMANAGERLISTENER_H_
#define DCPLUSPLUS_DCPP_FAVORITEMANAGERLISTENER_H_

#include "forward.h"
#include "noexcept.h"

namespace dcpp {

class FavoriteManagerListener {
public:
	virtual ~FavoriteManagerListener() { }
	template<int I>	struct X { enum { TYPE = I }; };

	typedef X<0> DownloadStarting;
	typedef X<1> DownloadFailed;
	typedef X<2> DownloadFinished;
	typedef X<3> FavoriteAdded;
	typedef X<4> FavoriteRemoved;
	typedef X<5> UserAdded;
	typedef X<6> UserUpdated;
	typedef X<7> UserRemoved;
	typedef X<8> StatusChanged;
	typedef X<9> LoadedFromCache;
	typedef X<10> Corrupted;
	typedef X<14> RecentAdded;
	typedef X<15> RecentRemoved;
	typedef X<16> RecentUpdated;
	//Indepent Fav
	typedef X<17> FavoriteIAdded;
	typedef X<18> FavoriteIRemoved;
	typedef X<19> FavoriteIUpdate;
	
	virtual void on(DownloadStarting, const string&) noexcept { }
	virtual void on(DownloadFailed, const string&) noexcept { }
	virtual void on(DownloadFinished, const string&, bool) noexcept { }
	virtual void on(FavoriteAdded, const FavoriteHubEntryPtr) noexcept { }
	virtual void on(FavoriteRemoved, const FavoriteHubEntryPtr) noexcept { }
	virtual void on(UserAdded, const FavoriteUser&) noexcept { }
	virtual void on(UserUpdated, const FavoriteUser&) noexcept { }
	virtual void on(UserRemoved, const FavoriteUser&) noexcept { }
	virtual void on(StatusChanged, const FavoriteUser&) noexcept { }
	virtual void on(LoadedFromCache, const string&, const string&) noexcept { }
	virtual void on(Corrupted, const string&) noexcept { }
	//Recent hubs
	virtual void on(RecentAdded, const RecentHubEntry* ) noexcept { }
	virtual void on(RecentRemoved, const RecentHubEntry* ) noexcept { }
	virtual void on(RecentUpdated, const RecentHubEntry* ) noexcept { }
	//Indepent Fav
	virtual void on(FavoriteIUpdate, const string&, FavoriteUser&) noexcept { }
	virtual void on(FavoriteIAdded, const string&, FavoriteUser*&) noexcept { }
	virtual void on(FavoriteIRemoved, const string&, FavoriteUser*&) noexcept { }
};

} // namespace dcpp

#endif
