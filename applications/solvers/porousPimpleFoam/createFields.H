#include "createRDeltaT.H"

Info<< "Reading field p_rgh\n" << endl;
volScalarField p_rgh
(
    IOobject
    (
        "p_rgh",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);

Info<< "Reading field U\n" << endl;
volVectorField U
(
    IOobject
    (
        "U",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);

Info<< "Reading transportProperties\n" << endl;

IOdictionary transportProperties
(
    IOobject
    (
        "transportProperties",
        runTime.constant(),
        mesh,
        IOobject::MUST_READ_IF_MODIFIED,
        IOobject::NO_WRITE
    )
);

#include "createPhi.H"

Info<< "Reading thermal diffusivity of fluid kf\n" << endl;
dimensionedScalar kf("kf", dimMass*dimLength/(dimTime*dimTime*dimTime*dimTemperature), transportProperties);

Info<< "Reading thermal diffusivity of stone kf\n" << endl;
dimensionedScalar ks("ks", dimMass*dimLength/(dimTime*dimTime*dimTime*dimTemperature), transportProperties);

Info<< "Reading heat tranfer coefficient, h (W/(m^2*K))\n" << endl;
dimensionedScalar h("h", dimless/dimTime, transportProperties);

Info<< "Reading specific heat capacity of fluid (J/kg*K)\n" << endl;
dimensionedScalar cpf("dp", dimLength*dimLength/(dimTemperature*dimTime*dimTime), transportProperties);

Info<< "Reading specific heat capacity of stone (J/kg*K)\n" << endl;
dimensionedScalar cps("dp", dimLength*dimLength/(dimTemperature*dimTime*dimTime), transportProperties);

Info<< "Reading stone radius (assuming spheres and uniform size (m)\n" << endl;
dimensionedScalar rp("rp", dimLength, transportProperties);

Info<< "Reading density of fluid (kg/m^3)\n" << endl;
dimensionedScalar rhof_value("rhof_value", dimDensity, transportProperties);

Info<< "Reading density of stone (kg/m^3)\n" << endl;
dimensionedScalar rhos_value("rhos_value", dimDensity, transportProperties);

volScalarField rhof
(
    IOobject
    (
        "rhof",
        runTime.timeName(),
        mesh,
        IOobject::READ_IF_PRESENT
    ),
    rhof_value
);

volScalarField rhos
(
    IOobject
    (
        "rhos",
        runTime.timeName(),
        mesh,
        IOobject::READ_IF_PRESENT
    ),
    rhos_value
);

// Mass flux
surfaceScalarField rhoPhi
(
    IOobject
    (
        "rhoPhi",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE
    ),
    fvc::interpolate(rhof)*phi
);

// typedef incompressibleInterPhaseTransportModel
//     <
//         immiscibleIncompressibleTwoPhaseMixture
//     > transportModelType;

autoPtr<transportModelType> turbulence
(
    new transportModelType
    (
        rho, U, phi, rhoPhi, mixture
    )
);

#include "readGravitationalAcceleration.H"
#include "readhRef.H"
#include "gh.H"


volScalarField p
(
    IOobject
    (
        "p",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::AUTO_WRITE
    ),
    p_rgh + rho*gh
);

label pRefCell = 0;
scalar pRefValue = 0.0;
setRefCell
(
    p,
    p_rgh,
    pimple.dict(),
    pRefCell,
    pRefValue
);

if (p_rgh.needReference())
{
    p += dimensionedScalar
    (
        "p",
        p.dimensions(),
        pRefValue - getRefCellValue(p, pRefCell)
    );
    p_rgh = p - rho*gh;
}

mesh.setFluxRequired(p_rgh.name());
mesh.setFluxRequired(alpha1.name());

#include "createMRF.H"
#include "createFvOptions.H"
#include "createPorosity.H"

isoAdvection advector(alpha1, phi, U);

Info<< "Reading field Tf\n" << endl;

volScalarField Tf
(
    IOobject
    (
        "Tf",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);

volScalarField Ts
(
    IOobject
    (
        "Ts",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);

volScalarField Astone
(
    IOobject
    (
        "Astone",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);