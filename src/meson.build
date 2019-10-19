midiman_sources = [
  'main.cpp',
  'midiman-window.cpp',
  'pianoroll-window.cpp'
]

midiman_deps = [
  dependency('gtkmm-3.0', version: '>= 3.18'),
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
