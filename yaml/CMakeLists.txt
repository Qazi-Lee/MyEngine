#yaml

file(GLOB YAML_SOURCES
"src/binary.cpp"


"src/convert.cpp"


"src/depthguard.cpp"


"src/directives.cpp"


"src/emit.cpp"


"src/emitfromevents.cpp"


"src/emitter.cpp"


"src/emitterstate.cpp"


"src/emitterutils.cpp"


"src/exceptions.cpp"


"src/exp.cpp"


"src/memory.cpp"


"src/node.cpp"


"src/node_data.cpp"


"src/nodebuilder.cpp"


"src/nodeevents.cpp"


"src/null.cpp"


"src/ostream_wrapper.cpp"


"src/parse.cpp"


"src/parser.cpp"


"src/regex_yaml.cpp"


"src/scanner.cpp"


"src/scanscalar.cpp"


"src/scantag.cpp"


"src/scantoken.cpp"


"src/simplekey.cpp"


"src/singledocparser.cpp"


"src/stream.cpp"


"src/tag.cpp"

)


add_library(yaml STATIC ${YAML_SOURCES})

target_compile_definitions(yaml PUBLIC YAML_CPP_STATIC_DEFINE)

target_include_directories(yaml PUBLIC 
${CMAKE_CURRENT_SOURCE_DIR}/include
)