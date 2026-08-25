#include "gtest/gtest.h"

#include "waspcore/wasp_math.h"

#include <vector>

using namespace wasp;

TEST(WaspMath, atom_fractions_and_mass_density_to_atom_density)
{
    const std::vector<double> molar_masses = {1.00782503223, 15.99491461957};
    const auto densities = compute_di_from_atom_fraction_mass_density(
        {2.0, 1.0}, molar_masses, 1.0);

    const double average_mass =
        (2.0 * molar_masses[0] + molar_masses[1]) / 3.0;
    EXPECT_NEAR((2.0 / 3.0) * avogadro * barn_to_cm2 / average_mass,
                densities[0], 1.0e-12);
    EXPECT_NEAR((1.0 / 3.0) * avogadro * barn_to_cm2 / average_mass,
                densities[1], 1.0e-12);
}

TEST(WaspMath, weight_fractions_and_mass_density_to_atom_density)
{
    const std::vector<double> molar_masses = {1.00782503223, 15.99491461957};
    const auto densities = compute_di_from_weight_fraction_mass_density(
        {0.1119, 0.8881}, molar_masses, 1.0);

    EXPECT_NEAR(0.1119 * avogadro * barn_to_cm2 / molar_masses[0],
                densities[0], 1.0e-12);
    EXPECT_NEAR(0.8881 * avogadro * barn_to_cm2 / molar_masses[1],
                densities[1], 1.0e-12);
}

TEST(WaspMath, atom_fractions_and_atomic_density_to_atom_density)
{
    const auto densities =
        compute_di_from_atom_fraction_atomic_density({2.0, 1.0}, 0.09);

    ASSERT_EQ(2u, densities.size());
    EXPECT_NEAR(0.06, densities[0], 1.0e-12);
    EXPECT_NEAR(0.03, densities[1], 1.0e-12);
}

TEST(WaspMath, weight_fractions_and_atomic_density_to_atom_density)
{
    const std::vector<double> molar_masses = {1.00782503223, 15.99491461957};
    const auto densities = compute_di_from_weight_fraction_atomic_density(
        {0.1119, 0.8881}, molar_masses, 0.1);
    const auto atom_fractions =
        weight_to_atom_fraction({0.1119, 0.8881}, molar_masses);

    ASSERT_EQ(2u, densities.size());
    EXPECT_NEAR(atom_fractions[0] * 0.1, densities[0], 1.0e-12);
    EXPECT_NEAR(atom_fractions[1] * 0.1, densities[1], 1.0e-12);
    EXPECT_NEAR(0.1, densities[0] + densities[1], 1.0e-12);
}

TEST(WaspMath, converted_atom_densities_reproduce_mass_density)
{
    const std::vector<double> molar_masses = {1.00782503223, 15.99491461957};
    const auto densities = compute_di_from_weight_fraction_mass_density(
        {0.1119, 0.8881}, molar_masses, 1.0);

    EXPECT_NEAR(1.0,
                compute_mass_density_from_atoms_bcm(densities, molar_masses),
                1.0e-12);
}
