import numpy as np


class BaseFilter(object):
    """This class implements base frame filter functionality
    """
    def __init__(self):
        pass

    def process_frame(self, frame:np.ndarray)->np.ndarray:
        """Makes frame transformation

        Args:
            frame (np.ndarray): input frame

        Raises:
            NotImplementedError: _description_

        Returns:
            np.ndarray: output frame
        """
        raise NotImplementedError()


__all__ = [ 'BaseFilter' ]
