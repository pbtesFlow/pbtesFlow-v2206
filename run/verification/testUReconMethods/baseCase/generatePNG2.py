#### import the simple module from the paraview
from paraview.simple import *
#### disable automatic camera reset on 'Show'
paraview.simple._DisableFirstRenderCameraReset()

# get active view
renderView1 = GetActiveViewOrCreate('RenderView')
# uncomment following to set a specific view size
# renderView1.ViewSize = [2201, 1132]

# destroy renderView1
Delete(renderView1)
del renderView1

# load state
LoadState('/home/roenby/Projects/pbtes-v2206/newPullDir3/pbtes-v2206/run/porousPimpleFoam/newUReconMethodTest/baseCase/state.pvsm', LoadStateDataFileOptions='Search files under specified directory',
    DataDirectory='/home/roenby/Projects/pbtes-v2206/newPullDir3/pbtes-v2206/run/porousPimpleFoam/newUReconMethodTest/baseCase',
    OnlyUseFilesInDataDirectory=0,
    casefoamFileName='/home/roenby/Projects/pbtes-v2206/newPullDir3/pbtes-v2206/run/porousPimpleFoam/newUReconMethodTest/standard/case.foam')

# get active view
renderView1 = GetActiveViewOrCreate('RenderView')
# uncomment following to set a specific view size
# renderView1.ViewSize = [2207, 580]

# find view
renderView2 = FindViewOrCreate('RenderView2', viewtype='RenderView')
# uncomment following to set a specific view size
# renderView2.ViewSize = [2207, 580]

# current camera placement for renderView1
renderView1.InteractionMode = '2D'
renderView1.CameraPosition = [4.0931824271415, 0.573769421487019, 16.1944370176042]
renderView1.CameraFocalPoint = [4.0931824271415, 0.573769421487019, 0.5]
renderView1.CameraParallelScale = 1.06965661156178

# current camera placement for renderView2
renderView2.InteractionMode = '2D'
renderView2.CameraPosition = [4.0931824271415, 0.573769421487019, 16.1944370176042]
renderView2.CameraFocalPoint = [4.0931824271415, 0.573769421487019, 0.5]
renderView2.CameraParallelScale = 1.06965661156178

# get layout
layout1 = GetLayout()

# save screenshot
SaveScreenshot('/home/roenby/Projects/pbtes-v2206/newPullDir3/pbtes-v2206/run/porousPimpleFoam/newUReconMethodTest/baseCase/overview2.png', layout1, SaveAllViews=1,
    ImageResolution=[2207, 1160],
    FontScaling='Do not scale fonts',
    SeparatorWidth=0,
    SeparatorColor=[0.0, 0.0, 0.0],
    OverrideColorPalette='',
    StereoMode='No change',
    TransparentBackground=0,
    ImageQuality=100)

#### saving camera placements for all active views

# current camera placement for renderView1
renderView1.InteractionMode = '2D'
renderView1.CameraPosition = [4.0931824271415, 0.573769421487019, 16.1944370176042]
renderView1.CameraFocalPoint = [4.0931824271415, 0.573769421487019, 0.5]
renderView1.CameraParallelScale = 1.06965661156178

# current camera placement for renderView2
renderView2.InteractionMode = '2D'
renderView2.CameraPosition = [4.0931824271415, 0.573769421487019, 16.1944370176042]
renderView2.CameraFocalPoint = [4.0931824271415, 0.573769421487019, 0.5]
renderView2.CameraParallelScale = 1.06965661156178

#### uncomment the following to render all views
# RenderAllViews()
# alternatively, if you want to write images, you can use SaveScreenshot(...).