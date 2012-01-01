/*
 * Copyright (C) 2011 - 2012 iceman50, iceman50@adcportal.com
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
 
 #ifndef DCPLUSPLUS_DCPP_BACKUP_MANAGER_H
#define DCPLUSPLUS_DCPP_BACKUP_MANAGER_H

#include <utility>

#include "Exception.h"
#include "File.h"
#include "LogManager.h"
#include "Semaphore.h"
#include "Singleton.h"
#include "Thread.h"
#include "TimerManager.h"
#include "Util.h"

namespace dcpp {

class BackupManager: public Singleton<BackupManager>, public Thread, public LogManagerListener, public SettingsManagerListener, private TimerManagerListener {
public:
	bool stop;
	CriticalSection cs;
	Semaphore s;
	
	virtual int run();
	void shutdown() {
			stop = true;
			s.signal();
		}
		
	void createBackup();
	
	virtual void on(TimerManagerListener::Minute, uint64_t aTick) noexcept;
	
private:
	friend class Singleton<BackupManager>;
	
	BackupManager() : stop(false) { }
	~BackupManager() throw() {
		shutdown();
		}
};

class RestoreManager: public Singleton<RestoreManager>, public Thread, public LogManagerListener, public SettingsManagerListener {
public:
	bool stop;
	CriticalSection cs;
	Semaphore s;
	
	virtual int run();
	void shutdown() {
			stop = true;
			s.signal();
		}
	
	void restoreBackup();

private:
	friend class Singleton<RestoreManager>;
	
	RestoreManager() : stop(false) {}
	~RestoreManager() throw() {
		shutdown();
		}
};

} //namespace dcpp

#endif