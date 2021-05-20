##############################
# Variables

set(IceSDKComponents
    IceCore
    IceSurface
    IceUtils
)

##############################
# Internals

foreach(C ${IceSDKComponents})
  add_subdirectory(Components/${C})
endforeach()

add_library(IceSDK INTERFACE)
    target_link_libraries(IceSDK INTERFACE ${IceSDKComponents})
