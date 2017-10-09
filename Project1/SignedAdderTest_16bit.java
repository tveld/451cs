import org.junit.Assert;
import org.junit.Test;

import static edu.gvsu.dlunit.DLUnit.*;

/**
 * Test cases for an Unsigned, 16-bit adder with a carry-in and overflow.
 * Created by kurmasz on 8/8/16.
 */
public class SignedAdderTest_16bit {


  // The complete list of integers to be tests.
  // (You need to add to this list.)
  public static final long testIntegers[] = {-32768, -32767, 0, 1, 2, 13, 127, 128, 129, 0x5555, 32766, 32767};


  private void verify(long a, long b, boolean carryIn) {

    long carryInAsInt = (carryIn ? 1 : 0);
    long expected = a + b + carryInAsInt;
    boolean expectedOverflow = ((expected >= (1 << 15)) || (expected < -(1 << 15)));

    setPinSigned("InputA", a);
    setPinSigned("InputB", b);
    setPin("CarryIn", carryIn);
    run();
    String message = "of " + a + " + " + b + " with " + (carryIn ? "a " : " no ") + " carry in";

    // Output "wraps around" if there is an overflow
    if (expectedOverflow && expected > 0) {
      expected -= 65536;
    } else if (expectedOverflow && expected < 0) {
      expected += 65536;
    }
    Assert.assertEquals("Output " + message, expected, readPinSigned("Output"));
    Assert.assertEquals("Overflow " + message, expectedOverflow, readPin("Overflow"));
  }

  //
  // Quick tests designed to quickly catch major errors.  (Also serve as example tests)
  //

  @Test
  public void zero_zero_false() {
    verify(0, 0, false);
  }

  @Test
  public void zero_one_false() {
    verify(0, 1, false);
  }

  @Test
  public void zero_zero_true() {
    verify(0, 0, true);
  }

  @Test
  public void zero_one_true() {
    verify(0, 1, true);
  }


  // This is actually rather gross; but, it is an effective way to thoroughly test your adder without
  // having to write hundreds of individual methods.
  @Test
  public void testAll() {
    for (long a : testIntegers) {
      for (long b : testIntegers) {
        verify(a, b, false);
        verify(a, b, true);
        verify(b, a, false);
        verify(b, a, true);
      }
    }
  } // end testAll
}
