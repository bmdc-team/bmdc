#!/usr/bin/python
# -*- coding: utf-8 -*-

import os
import commands
import string

#try:
#	from bzrlib import branch
#except ImportError:
#	print "bzrlib not installed"

EnsureSConsVersion(0, 98, 1)

PACKAGE = 'bmdc'
CORE_PACKAGE = 'libdcpp'
LIB_UPNP = 'libminiupnpc'
BUILD_PATH = '#/build/'
BUILD_LOCALE_PATH = BUILD_PATH + 'locale/'
LIB_IS_UPNP = True

# todo: remove -fpermissive and fix the errors
BUILD_FLAGS = {
	'common'  : ['-I#','-fpermissive','-D_GNU_SOURCE', '-D_LARGEFILE_SOURCE', '-D_FILE_OFFSET_BITS=64', '-D_REENTRANT' ],
	'debug'   : ['-g', '-ggdb', '-Wall', '-D_DEBUG'], 
	'release' : ['-O3', '-fomit-frame-pointer', '-DNDEBUG']
}

# ----------------------------------------------------------------------
# Function definitions
# ----------------------------------------------------------------------

def check_pkg_config(context):
	context.Message('Checking for pkg-config... ')
	ret = context.TryAction('pkg-config --version')[0]
	context.Result(ret)
	return ret

def check_pkg(context, name):
	context.Message('Checking for %s... ' % name)
	ret = context.TryAction('pkg-config --exists \'%s\'' % name)[0]
	context.Result(ret)
	return ret

def check_cxx_version(context, name, major, minor):
	context.Message('Checking for %s >= %d.%d...' % (name, major, minor))
	ret = commands.getoutput('%s -dumpversion' % name)

	retval = 0
	try:
		if ((string.atoi(ret[0]) == major and string.atoi(ret[2]) >= minor)
		or (string.atoi(ret[0]) > major)):
			retval = 1
	except ValueError:
		print "No C++ compiler found!"

	context.Result(retval)
	return retval

#def check_bzr_revision(context):
#	context.Message("Checking bzr revision...")
#	revision = ''

#	try:
#		b = branch.Branch.open('.')
#		revision = str(b.revno())
#	except:
#		print "failed"

#	context.env['BZR_REVISION'] = revision
#	context.Result(revision)
#	return revision
#
# Recursively installs all files within the source folder to target. Optionally,
# a filter function can be provided to prevent installation of certain files.
def recursive_install(env, source, target, filter = None):
	nodes = env.Glob(os.path.join(source, '*'))
	target = os.path.join(target, os.path.basename(source))

	for node in nodes:
		if node.isdir():
			env.RecursiveInstall(str(node), target, filter)
		elif filter == None or filter(node.name):
			env.Alias('install', env.Install(target, node))

def generate_message_catalogs(env):
	mo_path = os.path.join(BUILD_LOCALE_PATH, '%s', 'LC_MESSAGES', env['package'] + '.mo')
	po_files = env.Glob('po/*.po', strings = True)

	for po_file in po_files:
		basename = os.path.basename(po_file)
		lang = os.path.splitext(basename)[0]
		mo_file = mo_path % lang
		env.MoBuild(source = po_file, target = mo_file)

	return None

# ----------------------------------------------------------------------
# Command-line options
# ----------------------------------------------------------------------

# Parameters are only sticky from scons -> scons install, otherwise they're cleared.
if 'install' in COMMAND_LINE_TARGETS:
	vars = Variables('build/sconf/scache.conf')
else:
	vars = Variables()

vars.AddVariables(
	BoolVariable('debug', 'Compile the program with debug information', 0),
	BoolVariable('release', 'Compile the program with optimizations', 0),
	BoolVariable('profile', 'Compile the program with profiling information', 0),
	BoolVariable('libnotify', 'Enable notifications through libnotify', 1),
	BoolVariable('liblua', 'Enable Lua scripting', 1),
	BoolVariable('libgnome', 'Enable Gnome Libs', 1),
	PathVariable('PREFIX', 'Compile the program with PREFIX as the root for installation', '/usr/local', PathVariable.PathIsDir),
	('FAKE_ROOT', 'Make scons install the program under a fake root', '')
)

# ----------------------------------------------------------------------
# Initialization
# ----------------------------------------------------------------------

env = Environment(ENV = os.environ, variables = vars, package = PACKAGE)

env['mode'] = 'debug' if env.get('debug') else 'release'
env['build_path'] = BUILD_PATH + env['mode'] + '/'

if os.environ.has_key('CXX'):
	env['CXX'] = os.environ['CXX']
else:
	print 'CXX env variable is not set, attempting to use g++'
	env['CXX'] = 'g++'

if os.environ.has_key('CC'):
	env['CC'] = os.environ['CC']

if os.environ.has_key('CXXFLAGS'):
	env['CPPFLAGS'] = env['CXXFLAGS'] = os.environ['CXXFLAGS'].split()

if os.environ.has_key('LDFLAGS'):
	env['LINKFLAGS'] = os.environ['LDFLAGS'].split()

if os.environ.has_key('CFLAGS'):
	env['CFLAGS'] = os.environ['CFLAGS'].split()

env['CPPDEFINES'] = [] # Initialize as a list so Append doesn't concat strings

env.SConsignFile('build/sconf/.sconsign')
vars.Save('build/sconf/scache.conf', env)
Help(vars.GenerateHelpText(env))

pot_args = ['xgettext', '--default-domain=$PACKAGE', '--package-name=$PACKAGE',
		'--msgid-bugs-address=https://sourceforge.net/projects/freedcppmv/',
		'--copyright-holder=BMDC++ Team', '--add-comments=TRANSLATORS',
		'--keyword=_', '--keyword=N_', '--keyword=C_:1c,2', '--keyword=F_',
		'--keyword=P_:1,2', '--from-code=UTF-8', '--foreign-user',
		'--no-wrap', '--boost', '--sort-output', '--language=$LANGUAGE',
		'--output=$TARGET', '$SOURCES']
pot_build = Builder(action = Action([pot_args], 'Extracting messages to $TARGET from $SOURCES'))
env.Append(BUILDERS = {'PotBuild' : pot_build})

merge_pot_args = ['msgcat', '$SOURCES', '--output-file=$TARGET']
merge_pot_builder = Builder(action = Action([merge_pot_args], 'Merging pot files $SOURCES to $TARGET'))
env.Append(BUILDERS = {'MergePotFiles' : merge_pot_builder})

mo_args = ['msgfmt', '-c', '-o', '$TARGET', '$SOURCE']
mo_build = Builder(action = Action([mo_args], 'Compiling message catalog $TARGET from $SOURCES'))
env.Append(BUILDERS = {'MoBuild' : mo_build})

env.AddMethod(generate_message_catalogs, 'GenerateMessageCatalogs')
env.AddMethod(recursive_install, 'RecursiveInstall')

conf = env.Configure(
	custom_tests =
	{
		'CheckPKGConfig' : check_pkg_config,
		'CheckPKG' : check_pkg,
		'CheckCXXVersion' : check_cxx_version,
#		'CheckBZRRevision' : check_bzr_revision
	},
	conf_dir = 'build/sconf',
	log_file = 'build/sconf/config.log')

# ----------------------------------------------------------------------
# Dependencies
# ----------------------------------------------------------------------

if not 'install' in COMMAND_LINE_TARGETS:

	if not conf.CheckCXXVersion(env['CXX'], 4, 1):
		print 'Compiler version check failed. g++ 4.1 or later is needed'
		Exit(1)

	if not conf.CheckPKGConfig():
		print '\tpkg-config not found.'
		Exit(1)

	if not conf.CheckPKG('gtk+-2.0 >= 2.12'):
		print '\tgtk+ >= 2.12 not found.'
		print '\tNote: You might have the lib but not the headers'
		Exit(1)

	if not conf.CheckPKG('gthread-2.0 >= 2.4'):
		print '\tgthread >= 2.4 not found.'
		print '\tNote: You might have the lib but not the headers'
		Exit(1)

	if not conf.CheckPKG('libglade-2.0 >= 2.4'):
		print '\tlibglade-2.0 >= 2.4 not found.'
		print '\tNote: You might have the lib but not the headers'
		Exit(1)

	
	if not conf.CheckCXXHeader('boost/version.hpp', '<>'):
		print '\tboost not found.'
		print '\tNote: You might have the lib but not the headers'
		Exit(1)

	if not conf.CheckHeader('time.h'):
		Exit(1)

	if not conf.CheckHeader('signal.h'):
		Exit(1)

	if not conf.CheckHeader('unistd.h'):
		Exit(1)

	if not conf.CheckLibWithHeader('pthread', 'pthread.h', 'c'):
		print '\tpthread library not found'
		print '\tNote: You might have the lib but not the headers'
		Exit(1)

	if not conf.CheckLibWithHeader('z', 'zlib.h', 'c'):
		print '\tz library (gzip/z compression) not found'
		print '\tNote: You might have the lib but not the headers'
		Exit(1)

	if not conf.CheckLibWithHeader('bz2', 'bzlib.h', 'c'):
		print '\tbz2 library (bz2 compression) not found'
		print '\tNote: You might have the lib but not the headers'
		Exit(1)

	# This needs to be before ssl check on *BSD systems
	if not conf.CheckLib('crypto'):
		print '\tcrypto library not found'
		print '\tNote: This library may be a part of libssl on your system'
		Exit(1)

	if not conf.CheckLibWithHeader('ssl', 'openssl/ssl.h', 'c'):
		print '\tOpenSSL library (libssl) not found'
		print '\tNote: You might have the lib but not the headers'
		Exit(1)

	# Needed for XFlush(). Headers shouldn't be needed since we include gdk/gdkx.h
	if not conf.CheckLib('X11'):
		print '\tX11 library not found'
		Exit(1)

	if not conf.CheckHeader('iconv.h'):
		Exit(1)
	elif conf.CheckLibWithHeader('iconv', 'iconv.h', 'c', 'iconv(0, (const char **)0, 0, (char**)0, 0);'):
		conf.env.Append(CPPDEFINES = ('ICONV_CONST', 'const'))

	if conf.CheckHeader(['sys/types.h', 'sys/socket.h', 'ifaddrs.h', 'net/if.h']):
		conf.env.Append(CPPDEFINES = 'HAVE_IFADDRS_H')

	# TODO: Implement a plugin system so libnotify doesn't have compile-time dependencies
	if conf.env.get('libnotify'):
			if not conf.CheckPKG('libnotify >= 0.4.1'):
				print '\tlibnotify >= 0.4.1 not found, disabling notifications.'
				print '\tNote: You might have the lib but not the headers'
			else:
				conf.env.Append(CPPDEFINES = 'HAVE_LIBNOTIFY')
				conf.env.ParseConfig('pkg-config --libs libnotify')
				if conf.CheckPKG('libnotify >= 0.7'):
					conf.env.Append(CPPDEFINES = 'HAVE_LIBNOTIFY_0_7')
	
	#GNOME LIBs (use to Sounds)
	if conf.env.get('libgnome'):
		conf.env['HAVE_GNOME_LIB'] = 0
		if not conf.CheckPKG('libgnome-2.0 >= 2.0'):
			print '\tlibgnome >= 2.0 not found.'
			print '\tNote: You might have the lib but not the headers'
		else:
			conf.env['HAVE_GNOME_LIB'] = 1
	#lua	
	if conf.env.get('liblua'):
		conf.env['HAVE_LUA_H_B'] = 0
		conf.env['HAVE_LUA_H_51_B'] = 0
		if not conf.CheckPKG('lua > 5.0'):
			print '\tLua library not found'
			conf.env['HAVE_LUA_H_B'] = 1
		if not conf.CheckPKG('lua5.1 > 5.0'):
			print '\tLua5.1 library not found'
			conf.env['HAVE_LUA_H_51_B'] = 1
	
	# MiniUPnPc for UPnP
	if not conf.CheckLib('libminiupnpc'):
		LIB_IS_UPNP = False

	#	conf.CheckBZRRevision()
	env = conf.Finish()

# ----------------------------------------------------------------------
# Compile and link flags
# ----------------------------------------------------------------------

	env.MergeFlags(BUILD_FLAGS['common'])
	env.MergeFlags(BUILD_FLAGS[env['mode']])

	env.Append(CXXFLAGS = '-std=c++0x')
	env.Append(LINKFLAGS = ['-lboost_thread','-lboost_regex'])

	env.ParseConfig('pkg-config --libs libglade-2.0')
	env.ParseConfig('pkg-config --libs gthread-2.0')

	env.Append(LIBPATH = env['build_path'] + CORE_PACKAGE)
	env.Prepend(LIBS = 'dcpp')

	if os.sys.platform == 'linux2':
		env.Append(LINKFLAGS = '-Wl,--as-needed')

	if os.name == 'mac' or os.sys.platform == 'darwin':
		conf.env.Append(CPPDEFINES = ('ICONV_CONST', ''))

	if os.sys.platform == 'sunos5':
		conf.env.Append(CPPDEFINES = ('ICONV_CONST', 'const'))
		env.Append(LIBS = ['socket', 'nsl'])
		
	#LUA
	if conf.env.get('liblua'):
		if env['HAVE_LUA_H_B'] == 0:
			env.ParseConfig('pkg-config --cflags --libs lua')
			conf.env.Append(CPPDEFINES = ('-D_USELUA'))
		if env['HAVE_LUA_H_51_B'] == 0:
			env.ParseConfig('pkg-config --cflags --libs lua5.1')
			conf.env.Append(CPPDEFINES = ('-D_USELUA'))

	#gnome libs
	if conf.env.get('libgnome'):
		if env['HAVE_GNOME_LIB'] == 1:
			env.ParseConfig('pkg-config --cflags libgnome-2.0')
			env.ParseConfig('pkg-config --libs libgnome-2.0')
			conf.env.Append(CPPDEFINES = '-D_HAVEGNOME')

	if env.get('profile'):
		env.Append(CXXFLAGS = '-pg')
		env.Append(LINKFLAGS= '-pg')
	
	if not LIB_IS_UPNP:
		env.Append(LIBPATH = [BUILD_PATH + LIB_UPNP])
		env.Prepend(LIBS = [LIB_UPNP])
		
	if env.get('PREFIX'):
		data_dir = '\'\"%s/share\"\'' % env['PREFIX']
		env.Append(CPPDEFINES = ('_DATADIR', data_dir))


# ----------------------------------------------------------------------
# Build
# ----------------------------------------------------------------------
	# Build the miniupnpc library
	if not LIB_IS_UPNP:
		mini_env = env.Clone(package = LIB_UPNP)
		upnp = SConscript(dirs = 'miniupnpc', variant_dir = BUILD_PATH + LIB_UPNP, duplicate = 0, exports = {'env': mini_env})
	
	# Build the dcpp library
	dcpp_env = env.Clone(package = CORE_PACKAGE)
	libdcpp = SConscript(dirs = 'dcpp', variant_dir = env['build_path'] + CORE_PACKAGE, duplicate = 0, exports = {'env': dcpp_env})

	# Build the GUI
	ui_env = env.Clone()
	glade_pot_file = SConscript(dirs = 'glade', variant_dir = env['build_path'] + 'glade', duplicate = 0, exports = {'env': ui_env})

	(linux_pot_file, obj_files) = SConscript(dirs = 'linux', variant_dir = env['build_path'] + 'gui', duplicate = 0, exports = {'env': ui_env})

	# Create the executable
	if not LIB_IS_UPNP:
		env.Program(target = PACKAGE, source = [libdcpp, upnp,  obj_files])
	else:
		env.Program(target = PACKAGE, source = [libdcpp, obj_files])		

	# i18n
	env.MergePotFiles(source = [glade_pot_file, linux_pot_file], target = 'po/%s.pot' % PACKAGE)
	env.GenerateMessageCatalogs()

	# Build source files followed by everything else
	Default(PACKAGE, '.')

# ----------------------------------------------------------------------
# Install
# ----------------------------------------------------------------------

else:

	glade_files = env.Glob('glade/*.glade')
	text_files = env.Glob('*.txt')
	prefix = env['FAKE_ROOT'] + env['PREFIX']
	extfil = env.Glob('extensions/Scripts/*.sh')
	pythfil = env.Glob('extensions/Scripts/*.py')
	scriptsluafil = env.Glob('scripts/*.lua')
	country = env.Glob('country/*.png')
	desktop_file = os.path.join('data', PACKAGE + '.desktop')
	app_icon_filter = lambda icon: os.path.splitext(icon)[0] == PACKAGE
	regular_icon_filter = lambda icon: os.path.splitext(icon)[0] != PACKAGE

	env.RecursiveInstall('icons/hicolor', os.path.join(prefix, 'share', 'icons'), app_icon_filter)
	env.RecursiveInstall('icons/hicolor', os.path.join(prefix, 'share', PACKAGE, 'icons'), regular_icon_filter)
	env.RecursiveInstall(BUILD_LOCALE_PATH, os.path.join(prefix, 'share', 'locale'))

	env.Alias('install', env.Install(dir = os.path.join(prefix, 'share', PACKAGE, 'glade'), source = glade_files))
	env.Alias('install', env.Install(dir = os.path.join(prefix, 'share', 'doc', PACKAGE), source = text_files))
	env.Alias('install', env.Install(dir = os.path.join(prefix, 'share', 'applications'), source = desktop_file))
	env.Alias('install', env.Install(dir = os.path.join(prefix, 'share', PACKAGE, 'extensions/Scripts'), source = extfil))
	env.Alias('install', env.Install(dir = os.path.join(prefix, 'share', PACKAGE, 'extensions/Scripts'), source = pythfil))
	env.Alias('install', env.Install(dir = os.path.join(prefix, 'share', 'country'), source = country))
	env.Alias('install', env.Install(dir = os.path.join(prefix, 'bin'), source = PACKAGE))
	 
