import org.junit.Assert;
import org.junit.Test;

import static edu.gvsu.dlunit.DLUnit.*;

/**
 * Test cases for an Unsigned, 16-bit subtracter with overflow.
 * Created by kurmasz on 8/8/16.
 */
public class SignedMultiplyTest_16bit {


  // The complete list of integers to be tests.
  // (You need to add to this list.)
  public static final long testIntegers[] = {-32768, -32767, 0, 1, 2, 13, 127, 128, 129, 0x5555, 32766, 32767};


  private void verify(long a, long b) {

    long expected = a * b;
    boolean expectedOverflow = ((expected >= (1 << 15)) || (expected < -(1 << 15)));

    setPinSigned("InputA", a);
    setPinSigned("InputB", b);
    run();
    String message = String.format("of %d x %d: ", a, b);

    if (expectedOverflow) {
      Assert.assertEquals("Low " + message, expected & 0xFFFF, readPinUnsigned("Low"));
      Assert.assertEquals("High " + message, (expected >> 16) & 0x0FFFFL, readPinUnsigned("High"));
    } else {
      Assert.assertEquals("Low " + message, expected, readPinSigned("Low"));
      long expectedHigh = expected >= 0 ? 0 : -1;
      Assert.assertEquals("High " + message, expectedHigh, readPinSigned("High"));
    }
    Assert.assertEquals("Overflow " + message, expectedOverflow, readPin("Overflow"));
  }

  //
  // Quick tests designed to quickly catch major errors.  (Also serve as example tests)
  //

  @Test
  public void zero_zero() {
    verify(0, 0);
  }

  @Test
  public void zero_one() {
    verify(0, 1);
  }

  @Test
  public void example_overflow() {
    verify(10000, 10);
  }


  // This is actually rather gross; but, it is an effective way to thoroughly test your adder without
  // having to write hundreds of individual methods.
  @Test
  public void testAll() {
    for (long a : testIntegers) {
      for (long b : testIntegers) {
        verify(a, b);
        verify(a, b);
        verify(b, a);
        verify(b, a);
      }
    }
  } // end testAll
}
