    volScalarField dT(Ts-Tf);    

    // Fluid temperature equation
    Info << "Solving Tf equation" << endl;

    fvScalarMatrix TfEqn
    (
        fvm::ddt(porosity*rhof*cpf,Tf)
        + fvm::div(rhof*cpf*phi, Tf)
        - fvm::laplacian(kf, Tf)
        ==
        fvm::Su(h*(1-porosity)*3/rp*dT, Tf)
    );

    TfEqn.relax();
    fvOptions.constrain(TfEqn);
    TfEqn.solve();
    fvOptions.correct(Tf);

    // Updating temperature dependent fluid viscosity
    muf = mu + (mu2-mu)*(Tf-Tmu)/(Tmu2-Tmu);

    // Solid temperature equation
    Info << "Solving Ts equation" << endl;

    fvScalarMatrix TsEqn
    (
        fvm::ddt((1-porosity)*rhos*cps, Ts)
        - fvm::laplacian(ks, Ts)
        ==
        - fvm::Su(h*(1-porosity)*3/rp*dT, Ts)
    );

    TsEqn.relax();
    fvOptions.constrain(TsEqn);
    TsEqn.solve();
    fvOptions.correct(Ts);