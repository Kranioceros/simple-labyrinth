def FlagsForFile( filename, **kwargs ):
  return {
    'flags' : ['-Wall','-x', 'c','-I', 'src','-I', 'headers']}

