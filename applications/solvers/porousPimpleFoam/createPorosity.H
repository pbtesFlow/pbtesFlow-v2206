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

/*
const bool porosityEnabled
(
    porosityProperties.getOrDefault<bool>("porosityEnabled", false)
);


tmp<volScalarField> tporosity;
if (porosityEnabled)
{
    tporosity = tmp<volScalarField>::New
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
}
*/

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
