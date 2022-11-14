if (porosityEnabled)
{
    const volScalarField& porosity = tporosity.cref();

    fvScalarMatrix TfEqn
    (
        fvm::ddt(Tf)
        + fvm::div(phi, Tf)
        - fvm::laplacian(DTf, Tf)
        ==
        fvOptions(Tf)
        + (1-porosity)*Cf*(Ts-Tf)
    );

    TfEqn.relax();
    fvOptions.constrain(TfEqn);
    TfEqn.solve();
    fvOptions.correct(Tf);

    fvScalarMatrix TsEqn
    (
        fvm::ddt(Ts)
        - fvm::laplacian(DTs, Ts)
        ==
        fvOptions(Ts)
        + porosity*Cs*(Tf-Ts)
    );

    TsEqn.relax();
    fvOptions.constrain(TsEqn);
    TsEqn.solve();
    fvOptions.correct(Ts);
}