midiman_sources = [
  'main.cpp',
  'midiman-window.cpp',
  'pianoroll-window.cpp'
]

midifile_proj = subproject('midifile')

midiman_deps = [
  dependency('gtkmm-3.0', version: '>= 3.18'),
  midifile_proj.get_variable('libmidifile_dep')
]

gnome = import('gnome')

midiman_sources += gnome.compile_resources('midiman-resources',
  'midiman.gresource.xml',
  c_name: 'midiman'
)

executable('midiman', midiman_sources,
  dependencies: midiman_deps,
  install: true,
)
