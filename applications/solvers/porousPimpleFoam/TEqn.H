if (porosityEnabled)
{
    const volScalarField& porosity = tporosity.cref();

    fvScalarMatrix hfEqn
    (
        fvm::ddt(rhof*cpf,Tf)
        + fvm::div(fvc::interpolate(rhof*cpf)*phi, Tf)
        - fvm::laplacian(kf, Tf)
        ==
        fvOptions(Tf)
        + h*porosity.internalField()*mesh.V()*3.0/rp * (Tf-Ts)
    );

    hfEqn.relax();
    fvOptions.constrain(hfEqn);
    hfEqn.solve();
    fvOptions.correct(Tf);

    // fvScalarMatrix hsEqn
    // (
    //     fvm::ddt(rho*cpf,Ts)
    //     - fvm::laplacian(DTs, Ts)
    //     ==
    //     fvOptions(Ts)
    //    //+ porosity*Cs*(Tf-Ts)
    // );

    // TsEqn.relax();
    // fvOptions.constrain(TsEqn);
    // TsEqn.solve();
    // fvOptions.correct(Ts);
}