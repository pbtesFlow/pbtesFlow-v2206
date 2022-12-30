#### import the simple module from the paraview
from paraview.simple import *
#### disable automatic camera reset on 'Show'
paraview.simple._DisableFirstRenderCameraReset()

# get active view
renderView1 = GetActiveViewOrCreate('RenderView')
# uncomment following to set a specific view size
renderView1.ViewSize = [2201, 1069]

# destroy renderView1
Delete(renderView1)
del renderView1

# load state
LoadState('/home/roenby/Projects/pbtes-v2206/newPullDir3/pbtes-v2206/run/porousPimpleFoam/newUReconMethodTest/baseCase/state.pvsm', LoadStateDataFileOptions='Search files under specified directory',
    DataDirectory='/home/roenby/Projects/pbtes-v2206/newPullDir3/pbtes-v2206/run/porousPimpleFoam/newUReconMethodTest/baseCase',
    casefoamFileName='/home/roenby/Projects/pbtes-v2206/newPullDir3/pbtes-v2206/run/porousPimpleFoam/newUReconMethodTest/standard/case.foam')

# find source
casefoam = FindSource('case.foam')

# set active source
SetActiveSource(casefoam)

# get color transfer function/color map for 'U'
uLUT = GetColorTransferFunction('U')

# Rescale transfer function
uLUT.RescaleTransferFunction(5.99999809265, 6.00000047684)

# get opacity transfer function/opacity map for 'U'
uPWF = GetOpacityTransferFunction('U')

# Rescale transfer function
uPWF.RescaleTransferFunction(5.99999809265, 6.00000047684)

# find view
renderView2 = FindViewOrCreate('RenderView2', viewtype='RenderView')
# uncomment following to set a specific view size
# renderView2.ViewSize = [2201, 519]

# set active view
SetActiveView(renderView2)

# get color transfer function/color map for 'p'
pLUT = GetColorTransferFunction('p')

# Rescale transfer function
pLUT.RescaleTransferFunction(-20.6145038605, 2.83416843414)

# get opacity transfer function/opacity map for 'p'
pPWF = GetOpacityTransferFunction('p')

# Rescale transfer function
pPWF.RescaleTransferFunction(-20.6145038605, 2.83416843414)

# find view
renderView1 = FindViewOrCreate('RenderView1', viewtype='RenderView')
# uncomment following to set a specific view size
renderView1.ViewSize = [2207, 548]

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
SaveScreenshot('/home/roenby/Projects/pbtes-v2206/newPullDir3/pbtes-v2206/run/porousPimpleFoam/newUReconMethodTest/baseCase/overview.png', layout1, SaveAllViews=1,
    ImageResolution=[2207, 1096])

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