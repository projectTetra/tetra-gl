env = Environment();
env['CPPPATH'] = ['./inc'];
env['LIBPATH'] = ['./bin'];

env['LIBS'] = [];
env['CXX'] = 'clang++';
env['CXXFLAGS'] = [ '-std=c++11', '-ggdb' ];

buildLib = env.Library( './bin/tetraGl', Glob( 'src/*/*/*.cpp' ) );

env['LIBS'] += ['tetraGl'];

buildDemo = env.Program( './bin/demo.out' );
Depends( buildDemo, buildLib );

runDemo = Command( target = "runDemo", source = "./bin/demo.out",
                   action = ["./bin/demo.out"] );

Depends( runDemo, buildDemo );
Default( runDemo );
