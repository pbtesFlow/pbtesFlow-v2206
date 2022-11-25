Info<< "\nReading bedProperties" << endl;

IOdictionary bedProperties
(
    IOobject
    (
        "bedProperties",
        runTime.constant(),
        mesh,
        IOobject::MUST_READ_IF_MODIFIED,
        IOobject::NO_WRITE
    )
);

const dimensionedScalar rho_b("rho_b", dimDensity, bedProperties);
const dimensionedScalar cp_b("cp_b", dimLength*dimLength/(dimTime*dimTime*dimTemperature), bedProperties);