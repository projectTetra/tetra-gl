import os;
import os.path;

env = Environment();
env['CPPPATH'] = ['./inc', './depends/tetra-soil/src'];
env['LIBPATH'] = ['./bin', './depends/tetra-soil/bin'];

env['LIBS'] = [ 'tetraSoil' ];
env['CXX'] = 'clang++';
env['CXXFLAGS'] = [ '-std=c++11', '-ggdb' ];

buildLib = env.Library( './bin/tetraGl',
                        Glob( 'src/*/*/*.cpp' ) + 
                        Glob( 'src/*/*/*/*.cpp' ) );

env['LIBS'] += [
  'GL',
  'GLEW',
  'tetraGl',
  'sfml-graphics',
  'sfml-window',
  'sfml-system',
];

env['CPPPATH'] += [ './demo' ];

demoBaseDir = './demo';
demoNames = [o for o in os.listdir(demoBaseDir) 
              if o != "shaders" and o != "assets" and 
                os.path.isdir(os.path.join(demoBaseDir, o))];

for demoName in demoNames:
  demoDir = os.path.join( demoBaseDir, demoName );
  buildDemo = env.Program( './bin/' + demoName + '.out',
                           Glob( demoBaseDir + '/*.cpp') +
                           Glob( demoDir + '/*.cpp' ) );
  Depends( buildDemo, buildLib );
  Default( buildDemo );

