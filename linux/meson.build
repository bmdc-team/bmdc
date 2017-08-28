
bmdcpp_sources = files ( [
'AboutConfig.cc',
'AboutConfigFav.cc',
'adlsearch.cc',
'bacon-message-connection.cc',
'bookentry.cc',
'cmddebug.cc',
'detectiontab.cc',
'dialogentry.cc',
'diskinfo.cc',
'downloadqueue.cc',
'emoticons.cc',
'emoticonsdialog.cc',
'entry.cc',
'exportimport.cc',
'FavoriteHubDialog.cc',
'favoritehubs.cc',
'favoriteusers.cc',
'finishedtransfers.cc',
'freespace.cc',
'fsusage.cc',
'genres.cc',
'hashdialog.cc',
'hub.cc',
'ignoremenu.cc',
'IgnoreTempManager.cc',
'mainwindow.cc',
'notepad.cc',
'notify.cc',
'previewmenu.cc',
'privatemessage.cc',
'publichubs.cc',
'recenthub.cc',
'search.cc',
'SearchEntry.cc',
'searchspy.cc',
'settingsdialog.cc',
'settingsmanager.cc',
'sharebrowser.cc',
'ShellCommand.cc',
'sound.cc',
'stacktrace.cc',
'System.cc',
'transfers.cc',
'treeview.cc',
'uploadqueue.cc',
'UserCommandMenu.cc',
'wulfor.cc',
'wulformanager.cc',
'WulforUtil.cc'
])
pcre_dep  = dependency('libpcre')
thread_dep = dependency('threads')
gdeps = dependency('glib-2.0')
gtkdep = dependency('gtk+-3.0')
openssl = dependency('openssl')
bzipdep = cc.find_library('bz2', required : true)

executable('bmdc', sources: bmdcpp_sources, dependencies: [thread_dep, pcre_dep,openssl,bzipdep,gtkdep,gdeps],link_with: [libdcpp,libnatpmp, libminiupnpc],  install: true , cpp_args : ['-Wno-reorder', '-Wno-unused-value','-DUSE_STACKTRACE','-D_DATADIR="../share"'])


