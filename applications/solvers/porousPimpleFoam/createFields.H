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

#include "createPhi.H"

/*
Info<< "Reading transportProperties\n" << endl;
immiscibleIncompressibleTwoPhaseMixture mixture(U, phi);

volScalarField& alpha1(mixture.alpha1());
volScalarField& alpha2(mixture.alpha2());

const dimensionedScalar& rho1 = mixture.rho1();
const dimensionedScalar& rho2 = mixture.rho2();


// Need to store rho for ddt(rho, U)
volScalarField rho
(
    IOobject
    (
        "rho",
        runTime.timeName(),
        mesh,
        IOobject::READ_IF_PRESENT
    ),
    alpha1*rho1 + alpha2*rho2
);
rho.oldTime();


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
    fvc::interpolate(rho)*phi
);


typedef incompressibleInterPhaseTransportModel
    <
        immiscibleIncompressibleTwoPhaseMixture
    > transportModelType;

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
*/


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
    p_rgh// + rho*gh
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
    p_rgh = p;// - rho*gh;
}

mesh.setFluxRequired(p_rgh.name());
//mesh.setFluxRequired(alpha1.name());

#include "createMRF.H"
#include "createFvOptions.H"
#include "createPorosity.H"

//isoAdvection advector(alpha1, phi, U);

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

dimensionedScalar rhof("rhof", dimDensity, transportProperties);

dimensionedScalar rhos("rhos", dimDensity, transportProperties);

Info<< "Reading thermal diffusivity of fluid kf\n" << endl;
dimensionedScalar kf("kf", dimMass*dimLength/(dimTime*dimTime*dimTime*dimTemperature), transportProperties);

Info<< "Reading thermal diffusivity of stone kf\n" << endl;
dimensionedScalar ks("ks", dimMass*dimLength/(dimTime*dimTime*dimTime*dimTemperature), transportProperties);

//Info<< "Reading heat tranfer coefficient, h (W/(m^2*K))\n" << endl;
//dimensionedScalar h("h", dimless/dimTime, transportProperties);

Info<< "Reading specific heat capacity of fluid (J/kg*K)\n" << endl;
dimensionedScalar cpf("cpf", sqr(dimVelocity)/dimTemperature, transportProperties);

Info<< "Reading specific heat capacity of stone (J/kg*K)\n" << endl;
dimensionedScalar cps("cps", sqr(dimVelocity)/dimTemperature, transportProperties);

//Info<< "Reading stone radius (assuming spheres and uniform size (m)\n" << endl;
//dimensionedScalar rp("rp", dimLength, transportProperties);

Info<< "Reading Cf\n" << endl;

dimensionedScalar Cf("Cf", dimMass/dimLength/pow3(dimTime)/dimTemperature, transportProperties);

Info<< "Reading Cs\n" << endl;

dimensionedScalar Cs("Cs", dimMass/dimLength/pow3(dimTime)/dimTemperature, transportProperties);

dimensionedScalar mu("mu", dimDynamicViscosity, transportProperties);

dimensionedScalar Tmu
(
    "Tmu",
    dimTemperature,
    transportProperties.lookupOrDefault<scalar>("Tmu", Zero)
);

dimensionedScalar mu2
(
    "mu2",
    dimDynamicViscosity,
    transportProperties.lookupOrDefault<scalar>("mu2", Zero)
);

dimensionedScalar Tmu2
(
    "Tmu2",
    dimTemperature,
    transportProperties.lookupOrDefault<scalar>("Tmu2", Zero)
);

volScalarField muf
(
    IOobject
    (
        "muf",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE
    ),
    mu + (mu2-mu)*(Tf-Tmu)/(Tmu2-Tmu)
);

// NOTE: THIS IS NOT USED FOR ANYTHING BUT SOMEWHERE IT IS IN THE CODE A CALL TO
// lookupObject(rho) IS MADE. TRACK IT AND REMOVE IT!!!
volScalarField rho
(
    IOobject
    (
        "rho",
        runTime.timeName(),
        mesh,
        IOobject::READ_IF_PRESENT
    ),
    rhof*(1 + 0*muf/mu2)
);
