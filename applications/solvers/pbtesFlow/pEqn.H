{
    if (correctPhi)
    {
        rAU.ref() = 1.0/UEqn.A();
    }
    else
    {
        rAU = 1.0/UEqn.A();
    }

    surfaceScalarField rAUf("rAUf", fvc::interpolate(rAU()));
    volVectorField HbyA(constrainHbyA(rAU()*UEqn.H(), U, p_rgh));
    surfaceScalarField phiHbyA
    (
        "phiHbyA",
        fvc::flux(HbyA)
    );
    MRF.makeRelative(phiHbyA);

    if (p_rgh.needReference())
    {
        fvc::makeRelative(phiHbyA, U);
        adjustPhi(phiHbyA, U, p_rgh);
        fvc::makeAbsolute(phiHbyA, U);
    }

    // Update the pressure BCs to ensure flux consistency
    constrainPressure(p_rgh, U, phiHbyA, rAUf, MRF);

    while (pimple.correctNonOrthogonal())
    {
        fvScalarMatrix p_rghEqn
        (
            fvm::laplacian(rAUf, p_rgh) == fvc::div(phiHbyA)
        );

        p_rghEqn.setReference(pRefCell, getRefCellValue(p_rgh, pRefCell));

        p_rghEqn.solve(mesh.solver(p_rgh.select(pimple.finalInnerIter())));

        if (pimple.finalNonOrthogonalIter())
        {
            phi = phiHbyA - p_rghEqn.flux();

            p_rgh.relax();

            word UReconMethod
            (
                pimple.dict().lookupOrDefault<word>("UReconMethod", "standard")
            );

            Info << "Reconstructing U with method: " << UReconMethod << endl;

            if (UReconMethod == "standard")
            {
                U = HbyA - rAU()*fvc::reconstruct(p_rghEqn.flux()/rAUf);
            }
            else if (UReconMethod == "standard_norAU")
            {
                U = HbyA - fvc::reconstruct(p_rghEqn.flux());
            }
            else if (UReconMethod == "reconPhi")
            {
                U = fvc::reconstruct(phi);
            }
            else if (UReconMethod == "AguerreEtAl") 
            {
                bool UReconCorrect1stIter
                (
                    pimple.dict().lookupOrDefault("UReconCorrect1stIter", false)
                );

                if (nCorr > 0 || UReconCorrect1stIter)
                {
                    surfaceVectorField gradUfdotSf 
                        = fvc::interpolate(fvc::grad(U)) & mesh.Sf();
                    typedef typename outerProduct<vector, scalar>::type GradType;
                    surfaceVectorField SfHat(mesh.Sf()/mesh.magSf());
                    volVectorField Unew
                    (
                        IOobject
                        (
                            "Unew",
                            phi.instance(),
                            mesh,
                            IOobject::NO_READ,
                            IOobject::NO_WRITE
                        ),
                        inv(fvc::surfaceSum(SfHat*mesh.Sf()))
                            &
                            (
                                fvc::surfaceSum(SfHat*phi)
                            - fvc::surfaceSum(SfHat*(mesh.Cf() & gradUfdotSf))
                            + (mesh.C() & fvc::surfaceSum(gradUfdotSf*SfHat))
                            ),
                        extrapolatedCalculatedFvPatchField<GradType>::typeName
                    );
                    U = Unew;
                }
                else
                {
                    U = fvc::reconstruct(phi);
                }
            }

            U.correctBoundaryConditions();
            fvOptions.correct(U);
        }
    }

    #include "continuityErrs.H"

    // Correct Uf if the mesh is moving
    fvc::correctUf(Uf, U, phi);

    // Make the fluxes relative to the mesh motion
    fvc::makeRelative(phi, U);

    p == p_rgh;

    if (p_rgh.needReference())
    {
        p += dimensionedScalar
        (
            "p",
            p.dimensions(),
            pRefValue - getRefCellValue(p, pRefCell)
        );
        p_rgh = p;
    }

    if (!correctPhi)
    {
        rAU.clear();
    }
}
