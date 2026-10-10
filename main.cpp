
#include <iostream>
#include <iomanip>

// Structure required by Project 1.
// The arrays are allocated dynamically.
struct _capacitor
{
    double *time;
    double *voltage;
    double *current;
    double C;
};

typedef struct _capacitor Capacitor;

// Allocate memory for the capacitor arrays.
void allocate_memory(Capacitor &cap, int steps)
{
    cap.time = new double[steps + 1];
    cap.voltage = new double[steps + 1];
    cap.current = new double[steps + 1];
}

// Release dynamically allocated memory.
void free_memory(Capacitor &cap)
{
    delete[] cap.time;
    delete[] cap.voltage;
    delete[] cap.current;
}

// Simulate charging with a constant current source.
void constant_current(Capacitor &cap, int steps,
                      double dt, double source_current)
{
    // Initial conditions at timestep 0.
    cap.time[0] = 0.0;
    cap.voltage[0] = 0.0;
    cap.current[0] = source_current;

    // Calculate voltage and current at each timestep.
    for (int i = 1; i <= steps; i++)
    {
        cap.time[i] = cap.time[i - 1] + dt;

        cap.current[i] = source_current;

        // Finite-difference capacitor voltage equation.
        cap.voltage[i] = cap.voltage[i - 1]
                       + cap.current[i - 1] * dt / cap.C;
    }
}

// Simulate charging with a constant voltage source and resistor.
void constant_voltage(Capacitor &cap, int steps,
                      double dt, double R, double V0)
{
    // Initial conditions at timestep 0.
    cap.time[0] = 0.0;
    cap.current[0] = V0 / R;
    cap.voltage[0] = 0.0;

    // Calculate current and voltage at each timestep.
    for (int i = 1; i <= steps; i++)
    {
        cap.time[i] = cap.time[i - 1] + dt;

        // Finite-difference RC current equation.
        cap.current[i] = cap.current[i - 1]
                       - (cap.current[i - 1] / (R * cap.C)) * dt;

        // Kirchhoff's Voltage Law: V0 = VR + VC.
        cap.voltage[i] = V0 - cap.current[i] * R;
    }
}

// Print results every 200 timesteps.
void print_results(const Capacitor &cap, int steps)
{
    std::cout << std::scientific << std::setprecision(6);

    std::cout << "Step\tTime (s)\tVoltage (V)\tCurrent (A)\n";

    for (int i = 0; i <= steps; i += 200)
    {
        std::cout << i << "\t"
                  << cap.time[i] << "\t"
                  << cap.voltage[i] << "\t"
                  << cap.current[i] << "\n";
    }
}

int main()
{
    // Values specified in the project instructions.
    const double dt = 1e-10;
    const double final_time = 5e-6;
    const int steps = 50000;

    const double R = 1000.0;
    const double C = 100e-12;

    const double source_current = 1e-2;
    const double V0 = 10.0;

    // Create two capacitor structures.
    Capacitor current_cap;
    Capacitor voltage_cap;

    current_cap.C = C;
    voltage_cap.C = C;

    // Allocate arrays for both simulations.
    allocate_memory(current_cap, steps);
    allocate_memory(voltage_cap, steps);

    // Run both capacitor charging simulations.
    constant_current(current_cap, steps, dt, source_current);
    constant_voltage(voltage_cap, steps, dt, R, V0);

    std::cout << "ECE 231L - Project 1\n";
    std::cout << "Final time: " << final_time << " s\n";
    std::cout << "Number of timesteps: " << steps << "\n\n";

    std::cout << "CONSTANT CURRENT SOURCE\n";
    print_results(current_cap, steps);

    std::cout << "\nCONSTANT VOLTAGE SOURCE (RC CIRCUIT)\n";
    print_results(voltage_cap, steps);

    // Free all dynamically allocated memory.
    free_memory(current_cap);
    free_memory(voltage_cap);

    return 0;
}
