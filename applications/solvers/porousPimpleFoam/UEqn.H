    MRF.correctBoundaryVelocity(U);

    porosityf = upwind<scalar>(mesh, phi).interpolate(porosity);

    fvVectorMatrix UEqn
    (
        fvm::ddt(rhof, U) + fvm::div(rhof*phi/porosityf, U)
      - fvm::laplacian(muf, U)
    );

    #include "UEqnAddPorosity.H"

    UEqn.relax();

    fvOptions.constrain(UEqn);

    if (pimple.momentumPredictor())
    {
        solve
        (
            UEqn
         ==
            fvc::reconstruct
            (
                (
                  - fvc::snGrad(p_rgh)
                ) * mesh.magSf() //Correct with Sf here even with porosity? 
            )
        );

        fvOptions.correct(U);
    }
