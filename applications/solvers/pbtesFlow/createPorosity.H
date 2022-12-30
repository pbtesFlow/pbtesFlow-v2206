// Reading porosity properties from constant directory
IOdictionary porosityProperties
(
    IOobject
    (
        "porosityProperties",
        runTime.constant(),
        runTime,
        IOobject::READ_IF_PRESENT,
        IOobject::NO_WRITE
    )
);

volScalarField porosity
(
    IOobject
    (
        "porosity",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);

surfaceScalarField porosityf(fvc::interpolate(porosity));

{
    bool upwindPorosity
    (
        pimple.dict().lookupOrDefault<bool>("upwindPorosity", true)
    );

    if (upwindPorosity)
    {
        porosityf = upwind<scalar>(mesh, phi).interpolate(porosity);
    }
}