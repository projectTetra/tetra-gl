env = Environment();
env['CPPPATH'] = ['./inc'];
env['LIBPATH'] = ['./bin'];

env['LIBS'] = [];
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
buildDemo = env.Program( './bin/demo.out', Glob( 'demo/*.cpp' ) );
Depends( buildDemo, buildLib );
